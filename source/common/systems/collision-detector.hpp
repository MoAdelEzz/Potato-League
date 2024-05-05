#pragma once

#include "../components/rigid-body.hpp"
#include "../components/player-controller.hpp"
#include "../components/movement.hpp"
#include "../ecs/world.hpp"
#include <unordered_set>
#include <iostream>

using glm::vec4, glm::vec3, glm::vec4, glm::mat4, glm::distance;
using std::cout;
using std::unordered_set, std::vector, std::max, std::min;

vector<vec3> boundingBox = {
    vec3(1.0, 1.0, 1.0),
    vec3(-1.0, 1.0, 1.0), vec3(1.0, -1.0, 1.0), vec3(1.0, 1.0, -1.0),
    vec3(-1.0, -1.0, 1.0), vec3(1.0, -1.0, -1.0), vec3(-1.0, 1.0, -1.0),
    vec3(-1.0, -1.0, -1.0)};

namespace our
{
    class CollisionSystem
    {
        void moveInLocalSpace(Entity *A, float force, vec3 direction)
        {
            MovementComponent *movement = A->getComponent<MovementComponent>();
            movement->adjustSpeed(force);
            // movement->roll();
            movement->setForward(direction);
        }

        void dontMoveCamera(Entity *playerEntity)
        {
            MovementComponent *movement = playerEntity->getComponent<MovementComponent>();
            movement->stopMovingOneFrame = true;
        }

        // intermediate variables between functions
        Entity *AOwner, *BOwner;
        mat4 a_local_to_world, b_local_to_world;
        vec3 center_a, center_b;

        vector<vec3> generateTransformedCube(mat4 transformationMatrix)
        {
            vector<vec3> generatedCube;
            for (vec3 &vertex : boundingBox)
            {
                generatedCube.push_back(transformationMatrix * vec4(vertex, 1.0));
            }
            return generatedCube;
        }

        // ==============================================================================
        vector<vec3> generateTestAxis(vector<vec3> A_axis, vector<vec3> B_axis)
        {
            vector<vec3> normalizedAxis(A_axis);
            normalizedAxis.insert(normalizedAxis.end(), B_axis.begin(), B_axis.end());
            for (auto &vecA : A_axis)
            {
                for (auto &vecB : B_axis)
                {
                    vec3 test = glm::cross(vecA, vecB);
                    if (glm::length(test) == 0)
                        continue;
                    normalizedAxis.push_back(glm::normalize(test));
                }
            }
            return normalizedAxis;
        }

        float isCollided(RigidBodyComponent *A, RigidBodyComponent *B, bool totalCollision = false)
        {
            AOwner = A->getOwner();
            BOwner = B->getOwner();

            a_local_to_world = AOwner->getLocalToWorldMatrix();
            b_local_to_world = BOwner->getLocalToWorldMatrix();

            center_a = getTransitionComponent(a_local_to_world);
            center_b = getTransitionComponent(b_local_to_world);

            vector<vec3> normalizedAxis = generateTestAxis(A->getBoxNormals(a_local_to_world), B->getBoxNormals(b_local_to_world));

            float min_overlap = (float)INT32_MAX;
            for (auto &axis : normalizedAxis)
            {
                vector<float> AProjectionLimits = A->getProjectionRange(axis, a_local_to_world);
                vector<float> BProjectionLimits = B->getProjectionRange(axis, b_local_to_world);

                // printf("\n\n\n\n\n\n\n\n\n\n\n");
                // if ((A->tag == CAR || A->tag == WALL) && (B->tag == CAR || B->tag == WALL))
                // {
                //     printf("axis (%0.08f, %0.08f, %0.08f)\n", axis[0], axis[1], axis[2]);
                //     printf("First Projection Limits: (%0.08f, %0.08f)\n", AProjectionLimits[0], AProjectionLimits[1]);
                //     printf("Second Projection Limits: (%0.08f, %0.08f)\n\n\n", BProjectionLimits[0], BProjectionLimits[1]);
                // }
                // printf("\n\n\n\n\n\n\n\n\n\n\n");

                // to make make the first point belongs to A for
                if (AProjectionLimits[0] > BProjectionLimits[1])
                    swap(AProjectionLimits, BProjectionLimits);

                if (AProjectionLimits[1] < BProjectionLimits[0])
                    return 0;
            }

            return min_overlap;
        }

        bool CarHitsBall(RigidBodyComponent *car, RigidBodyComponent *ball)
        {
            if (car->tag == BALL)
                std::swap(car, ball);

            float overlap = isCollided(car, ball);
            if (overlap)
            {
                vec3 normal = vec3(0., 0., 0.);

                vec3 forward = car->getOwner()->getComponent<MovementComponent>()->forward;

                if (glm::length(center_b - center_a) != 0)
                    normal = glm::normalize(center_b - center_a);
                else
                    return false;

                float cosAngle = abs(glm::dot(normal, forward)) * 0.25f;
                dontMoveCamera(AOwner);
                moveInLocalSpace(BOwner, 20 * cosAngle, normal);

                return true;
                // BOwner->localTransform.position += normal * 0.1f;
                // ball->getOwner()->getComponent<MovementComponent>()->lastWallNormal = vec3(0,0,0);
            }
            return false;
        }

        bool CarHitsBomb(RigidBodyComponent *car, RigidBodyComponent *bomb)
        {
            if (car->tag == BOMB)
                std::swap(car, bomb);

            if (isCollided(car, bomb))
            {
                printf("car bomb collided\n");
                // vec3 normal = vec3(0., 0., 0.);

                // if (glm::length(center_b - center_a) != 0)
                //     normal = glm::normalize(center_b - center_a);
                // else
                //     return;

                // dontMoveCamera(AOwner);
                // moveInLocalSpace(BOwner, 6, normal);

                return true;
            }
            return false;
        }

        bool BallHitsBomb(RigidBodyComponent *ball, RigidBodyComponent *bomb)
        {
            if (ball->tag == BOMB)
                std::swap(ball, bomb);

            if (isCollided(ball, bomb))
            {
                printf("ball bomb collided\n");
                // vec3 normal = vec3(0., 0., 0.);

                // if (glm::length(center_b - center_a) != 0)
                //     normal = glm::normalize(center_b - center_a);
                // else
                //     return;

                // dontMoveCamera(AOwner);
                // moveInLocalSpace(BOwner, 6, normal);
                return true;
            }
            return false;
        }

        void BallHitsWall(RigidBodyComponent *ball, RigidBodyComponent *wall)
        {
            if (ball->tag == WALL)
                std::swap(ball, wall);

            if (isCollided(ball, wall))
            {
                vec3 normal = resolveWallNormal(wall);

                printf("normal: (%0.08f, %0.08f, %0.08f)\n", normal[0], normal[1], normal[2]);

                MovementComponent *ballMovement = AOwner->getComponent<MovementComponent>();

                // if (ballMovement->lastWallNormal == normal)
                //     return;

                // ballMovement->lastWallNormal = normal;

                float velocity = ballMovement->current_velocity;
                vec3 normalizedVelocity = ballMovement->forward;

                vec3 reflectionVec = normalizedVelocity - 2.0f * glm::dot(normal, normalizedVelocity) * normal;
                if (glm::length(reflectionVec) != 0)
                    reflectionVec = glm::normalize(reflectionVec);

                // printf("normalized: (%0.08f, %0.08f, %0.08f)\n", normalizedVelocity[0], normalizedVelocity[1], normalizedVelocity[2]);
                // printf("reflectionVec: (%0.08f, %0.08f, %0.08f)\n", reflectionVec[0], reflectionVec[1], reflectionVec[2]);

                ballMovement->setForward(reflectionVec);
                // TODO: change ball velocity
            }
        }

        void CarHitsWall(RigidBodyComponent *car, RigidBodyComponent *wall)
        {
            if (car->tag == WALL)
                std::swap(car, wall);

            if (isCollided(car, wall))
            {
                vec3 normal = resolveWallNormal(wall);
                MovementComponent *carMovement = AOwner->getComponent<MovementComponent>();

                carMovement->current_velocity = 0;
                carMovement->collidedWallNormal = normal;
                // carMovement->stopMovingOneFrame = true;
            }
        }

        bool VarCheckPossibleGoal(RigidBodyComponent *ball, RigidBodyComponent *goal)
        {
            if (ball->tag == Tag::WALL)
                std::swap(ball, goal);

            bool applyTotalCollision = true;
            if (isCollided(ball, goal, applyTotalCollision))
            {
                // TODO: we must do something to declare winning
                std::cout << "YAY" << std::endl;
                return true;
            }
            return false;
        }

        vec3 resolveWallNormal(RigidBodyComponent *wall)
        {
            vec3 normal;
            switch (wall->wallType)
            {
            case WallType::LEFT:
                normal = vec3(1, 0, 0);
                break;
            case WallType::RIGHT:
                normal = vec3(-1, 0, 0);
                break;
            case WallType::TOP:
                normal = vec3(0, -1, 0);
                break;
            case WallType::DOWN:
                normal = vec3(0, 1, 0);
                break;
            case WallType::FRONT:
                normal = vec3(0, 0, 1);
                break;
            case WallType::BACK:
                normal = vec3(0, 0, -1);
                break;
            default:
                normal = vec3(0.0f, 0.0f, 0.0f);
                break;
            }

            // Transform t; t.rotation.y = wall->getOwner()->localTransform.rotation.y;
            // normal = t.toMat4() * vec4(normal, 0.0f);

            return normal;
        }

    public:
        static vec3 getTransitionComponent(mat4 translationMatrix)
        {
            return vec3(translationMatrix[3][0], translationMatrix[3][1], translationMatrix[3][2]);
        }

        void checkForCollisions(World *world)
        {
            unordered_set<Entity *> entities = world->getEntities();
            vector<RigidBodyComponent *> rigidBodies;
            for (Entity *entity : entities)
            {
                RigidBodyComponent *rigidBody = entity->getComponent<RigidBodyComponent>();
                if (rigidBody == nullptr)
                    continue;
                rigidBodies.push_back(rigidBody);
            }
            for (unsigned int i = 0; i < rigidBodies.size(); i++)
            {
                bool iIsBall = rigidBodies[i]->tag == BALL;
                bool iIsCar = rigidBodies[i]->tag == CAR;
                bool iIsWall = rigidBodies[i]->tag == WALL;
                bool iIsGround = rigidBodies[i]->tag == GROUND;
                bool iIsGoal = rigidBodies[i]->tag == GOAL;
                bool iIsObstacle = rigidBodies[i]->tag == NONE; // TODO: change this to obstacle
                bool iIsBomb = rigidBodies[i]->tag == BOMB;

                for (unsigned int j = i + 1; j < rigidBodies.size(); j++)
                {
                    bool isBall = iIsBall || rigidBodies[j]->tag == BALL;
                    bool isCar = iIsCar || rigidBodies[j]->tag == CAR;
                    bool isWall = iIsWall || rigidBodies[j]->tag == WALL;
                    bool isGround = iIsGround || rigidBodies[j]->tag == GROUND;
                    bool isGoal = iIsGoal || rigidBodies[j]->tag == GOAL;
                    bool isObstacle = iIsObstacle || rigidBodies[j]->tag == NONE; // TODO: change this to obstacle
                    bool isBomb = iIsBomb || rigidBodies[j]->tag == BOMB;

                    if (isBall && isCar)
                        CarHitsBall(rigidBodies[i], rigidBodies[j]);
                    if (isBall && isWall)
                        BallHitsWall(rigidBodies[i], rigidBodies[j]);
                    if (isCar && isWall)
                        CarHitsWall(rigidBodies[i], rigidBodies[j]);
                    if (isGoal && isBall)
                        VarCheckPossibleGoal(rigidBodies[i], rigidBodies[j]);
                }
            }
        }

        bool checkForBombCollision(World *world)
        {
            bool carHitsBombCheck = false;
            bool ballHitsBombCheck = false;
            unordered_set<Entity *> entities = world->getEntities();
            vector<RigidBodyComponent *> rigidBodies;
            for (Entity *entity : entities)
            {
                RigidBodyComponent *rigidBody = entity->getComponent<RigidBodyComponent>();
                if (rigidBody == nullptr)
                    continue;
                rigidBodies.push_back(rigidBody);
            }
            for (unsigned int i = 0; i < rigidBodies.size(); i++)
            {
                bool iIsBall = rigidBodies[i]->tag == BALL;
                bool iIsCar = rigidBodies[i]->tag == CAR;
                bool iIsWall = rigidBodies[i]->tag == WALL;
                bool iIsGround = rigidBodies[i]->tag == GROUND;
                bool iIsGoal = rigidBodies[i]->tag == GOAL;
                bool iIsObstacle = rigidBodies[i]->tag == NONE; // TODO: change this to obstacle
                bool iIsBomb = rigidBodies[i]->tag == BOMB;

                for (unsigned int j = i + 1; j < rigidBodies.size(); j++)
                {
                    bool isBall = iIsBall || rigidBodies[j]->tag == BALL;
                    bool isCar = iIsCar || rigidBodies[j]->tag == CAR;
                    bool isWall = iIsWall || rigidBodies[j]->tag == WALL;
                    bool isGround = iIsGround || rigidBodies[j]->tag == GROUND;
                    bool isGoal = iIsGoal || rigidBodies[j]->tag == GOAL;
                    bool isObstacle = iIsObstacle || rigidBodies[j]->tag == NONE; // TODO: change this to obstacle
                    bool isBomb = iIsBomb || rigidBodies[j]->tag == BOMB;

                    if (isBomb && isCar)
                        if (CarHitsBomb(rigidBodies[i], rigidBodies[j]))
                        {
                            carHitsBombCheck = true;
                            break;
                        }
                    if (isBomb && isBall)
                        if (BallHitsBomb(rigidBodies[i], rigidBodies[j]))
                        {
                            ballHitsBombCheck = true;
                            break;
                        }
                }
                if (carHitsBombCheck || ballHitsBombCheck)
                {
                    break;
                }
            }
            return (carHitsBombCheck || ballHitsBombCheck);
        }

        bool checkForGoal(World *world)
        {
            bool goalCheck = false;
            unordered_set<Entity *> entities = world->getEntities();
            vector<RigidBodyComponent *> rigidBodies;
            for (Entity *entity : entities)
            {
                RigidBodyComponent *rigidBody = entity->getComponent<RigidBodyComponent>();
                if (rigidBody == nullptr)
                    continue;
                rigidBodies.push_back(rigidBody);
            }
            for (unsigned int i = 0; i < rigidBodies.size(); i++)
            {
                bool iIsBall = rigidBodies[i]->tag == BALL;
                bool iIsCar = rigidBodies[i]->tag == CAR;
                bool iIsWall = rigidBodies[i]->tag == WALL;
                bool iIsGround = rigidBodies[i]->tag == GROUND;
                bool iIsGoal = rigidBodies[i]->tag == GOAL;
                bool iIsObstacle = rigidBodies[i]->tag == NONE; // TODO: change this to obstacle
                bool iIsBomb = rigidBodies[i]->tag == BOMB;

                for (unsigned int j = i + 1; j < rigidBodies.size(); j++)
                {
                    bool isBall = iIsBall || rigidBodies[j]->tag == BALL;
                    bool isCar = iIsCar || rigidBodies[j]->tag == CAR;
                    bool isWall = iIsWall || rigidBodies[j]->tag == WALL;
                    bool isGround = iIsGround || rigidBodies[j]->tag == GROUND;
                    bool isGoal = iIsGoal || rigidBodies[j]->tag == GOAL;
                    bool isObstacle = iIsObstacle || rigidBodies[j]->tag == NONE; // TODO: change this to obstacle
                    bool isBomb = iIsBomb || rigidBodies[j]->tag == BOMB;

                    if (isGoal && isBall)
                        if (VarCheckPossibleGoal(rigidBodies[i], rigidBodies[j]))
                        {
                            goalCheck = true;
                            break;
                        }
                }
                if (goalCheck)
                {
                    break;
                }
            }
            return (goalCheck);
        }

        bool checkForBallCollision(World *world)
        {
            bool ballHitsCar = false;
            unordered_set<Entity *> entities = world->getEntities();
            vector<RigidBodyComponent *> rigidBodies;
            for (Entity *entity : entities)
            {
                RigidBodyComponent *rigidBody = entity->getComponent<RigidBodyComponent>();
                if (rigidBody == nullptr)
                    continue;
                rigidBodies.push_back(rigidBody);
            }
            for (unsigned int i = 0; i < rigidBodies.size(); i++)
            {
                bool iIsBall = rigidBodies[i]->tag == BALL;
                bool iIsCar = rigidBodies[i]->tag == CAR;
                bool iIsWall = rigidBodies[i]->tag == WALL;
                bool iIsGround = rigidBodies[i]->tag == GROUND;
                bool iIsGoal = rigidBodies[i]->tag == GOAL;
                bool iIsObstacle = rigidBodies[i]->tag == NONE; // TODO: change this to obstacle
                bool iIsBomb = rigidBodies[i]->tag == BOMB;

                for (unsigned int j = i + 1; j < rigidBodies.size(); j++)
                {
                    bool isBall = iIsBall || rigidBodies[j]->tag == BALL;
                    bool isCar = iIsCar || rigidBodies[j]->tag == CAR;
                    bool isWall = iIsWall || rigidBodies[j]->tag == WALL;
                    bool isGround = iIsGround || rigidBodies[j]->tag == GROUND;
                    bool isGoal = iIsGoal || rigidBodies[j]->tag == GOAL;
                    bool isObstacle = iIsObstacle || rigidBodies[j]->tag == NONE; // TODO: change this to obstacle
                    bool isBomb = iIsBomb || rigidBodies[j]->tag == BOMB;

                    if (isCar && isBall)
                        if (CarHitsBall(rigidBodies[i], rigidBodies[j]))
                        {
                            ballHitsCar = true;
                            break;
                        }
                }
                if (ballHitsCar)
                {
                    break;
                }
            }
            return ballHitsCar;
        }
    };

}