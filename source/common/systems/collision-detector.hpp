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

vector<vec3> unityCube = {
    vec3(1.0, 1.0, 1.0),
    vec3(-1.0, 1.0, 1.0), vec3(1.0, -1.0, 1.0), vec3(1.0, 1.0, -1.0),
    vec3(-1.0, -1.0, 1.0), vec3(1.0, -1.0, -1.0), vec3(-1.0, 1.0, -1.0),
    vec3(-1.0, -1.0, -1.0)};

namespace our
{
    class CollisionSystem
    {
        vec3 getTransitionComponent(mat4 translationMatrix)
        {
            return vec3(translationMatrix[3][0], translationMatrix[3][1], translationMatrix[3][2]);
        }

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

        bool cubesCollision(const vector<vec3> &a_vertices, const vector<vec3> &b_vertices)
        {
            vec3 a_maxCoordinates = vec3(INT32_MIN, INT32_MIN, INT32_MIN);
            vec3 a_minCoordinates = vec3(INT32_MAX, INT32_MAX, INT32_MAX);

            vec3 b_maxCoordinates = vec3(INT32_MIN, INT32_MIN, INT32_MIN);
            vec3 b_minCoordinates = vec3(INT32_MAX, INT32_MAX, INT32_MAX);

            for (vec3 point : a_vertices)
            {
                a_maxCoordinates.x = max(a_maxCoordinates.x, point.x), a_maxCoordinates.y = max(a_maxCoordinates.y, point.y), a_maxCoordinates.z = max(a_maxCoordinates.z, point.z);
                a_minCoordinates.x = min(a_minCoordinates.x, point.x), a_minCoordinates.y = min(a_minCoordinates.y, point.y), a_minCoordinates.z = min(a_minCoordinates.z, point.z);
            }

            for (vec3 point : b_vertices)
            {
                b_maxCoordinates.x = max(b_maxCoordinates.x, point.x), b_maxCoordinates.y = max(b_maxCoordinates.y, point.y), b_maxCoordinates.z = max(b_maxCoordinates.z, point.z);
                b_minCoordinates.x = min(b_minCoordinates.x, point.x), b_minCoordinates.y = min(b_minCoordinates.y, point.y), b_minCoordinates.z = min(b_minCoordinates.z, point.z);
            }

            bool overlapX = a_minCoordinates.x >= b_minCoordinates.x && a_minCoordinates.x <= b_maxCoordinates.x;
            overlapX |= b_minCoordinates.x >= a_minCoordinates.x && b_minCoordinates.x <= a_maxCoordinates.x;

            bool overlapY = a_minCoordinates.y >= b_minCoordinates.y && a_minCoordinates.y <= b_maxCoordinates.y;
            overlapY |= b_minCoordinates.y >= a_minCoordinates.y && b_minCoordinates.y <= a_maxCoordinates.y;

            bool overlapZ = a_minCoordinates.z >= b_minCoordinates.z && a_minCoordinates.z <= b_maxCoordinates.z;
            overlapZ |= b_minCoordinates.z >= a_minCoordinates.z && b_minCoordinates.z <= a_maxCoordinates.z;

            return overlapX && overlapY && overlapZ;
        }

        bool isAInsideB(const vector<vec3> &a_vertices, const vector<vec3> &b_vertices)
        {
            vec3 a_maxCoordinates = vec3(INT32_MIN, INT32_MIN, INT32_MIN);
            vec3 a_minCoordinates = vec3(INT32_MAX, INT32_MAX, INT32_MAX);

            vec3 b_maxCoordinates = vec3(INT32_MIN, INT32_MIN, INT32_MIN);
            vec3 b_minCoordinates = vec3(INT32_MAX, INT32_MAX, INT32_MAX);

            for (vec3 point : a_vertices)
            {
                a_maxCoordinates.x = max(a_maxCoordinates.x, point.x), a_maxCoordinates.y = max(a_maxCoordinates.y, point.y), a_maxCoordinates.z = max(a_maxCoordinates.z, point.z);
                a_minCoordinates.x = min(a_minCoordinates.x, point.x), a_minCoordinates.y = min(a_minCoordinates.y, point.y), a_minCoordinates.z = min(a_minCoordinates.z, point.z);
            }

            for (vec3 point : b_vertices)
            {
                b_maxCoordinates.x = max(b_maxCoordinates.x, point.x), b_maxCoordinates.y = max(b_maxCoordinates.y, point.y), b_maxCoordinates.z = max(b_maxCoordinates.z, point.z);
                b_minCoordinates.x = min(b_minCoordinates.x, point.x), b_minCoordinates.y = min(b_minCoordinates.y, point.y), b_minCoordinates.z = min(b_minCoordinates.z, point.z);
            }

            bool overlapX = a_minCoordinates.x >= b_minCoordinates.x && a_maxCoordinates.x <= b_maxCoordinates.x;

            bool overlapY = a_minCoordinates.y >= b_minCoordinates.y && a_maxCoordinates.y <= b_maxCoordinates.y;

            bool overlapZ = a_minCoordinates.z >= b_minCoordinates.z && a_maxCoordinates.z <= b_maxCoordinates.z;

            return overlapX && overlapY && overlapZ;
        }

        // intermediate variables between functions
        Entity *AOwner, *BOwner;
        mat4 a_local_to_world, b_local_to_world;
        vec3 center_a, center_b;

        vector<vec3> generateTransformedCube(mat4 transformationMatrix)
        {
            vector<vec3> generatedCube;
            for (vec3 &vertex : unityCube)
            {
                generatedCube.push_back(transformationMatrix * vec4(vertex, 1.0));
            }
            return generatedCube;
        }

        bool isCollided(RigidBodyComponent *A, RigidBodyComponent *B, bool totalCollision = false)
        {
            AOwner = A->getOwner();
            BOwner = B->getOwner();

            a_local_to_world = AOwner->getLocalToWorldMatrix();
            b_local_to_world = BOwner->getLocalToWorldMatrix();

            center_a = getTransitionComponent(a_local_to_world);
            center_b = getTransitionComponent(b_local_to_world);

            bool isCollided = false;

            if (A->bodyType == CUBE && B->bodyType == CUBE)
            {
                unityCube = A->unityCube;
                const vector<vec3> AVertices = generateTransformedCube(a_local_to_world);

                unityCube = B->unityCube;
                const vector<vec3> BVertices = generateTransformedCube(b_local_to_world);

                isCollided = totalCollision ? isAInsideB(AVertices, BVertices) : cubesCollision(AVertices, BVertices);
            }
            else
            {
                // cout << distance(center_a, center_b) << std::endl;
                isCollided = distance(center_a, center_b) < 2;
            }

            return isCollided;
        }

        void CarHitsBall(RigidBodyComponent *car, RigidBodyComponent *ball)
        {
            if (car->tag == BALL)
                std::swap(car, ball);

            if (isCollided(car, ball))
            {
                printf("collided\n");
                vec3 normal = vec3(0., 0., 0.);

                if (glm::length(center_b - center_a) != 0)
                    normal = glm::normalize(center_b - center_a);
                else
                    return;

                dontMoveCamera(AOwner);
                moveInLocalSpace(BOwner, 6, normal);
            }
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
                // we should get the normal of the wall
                // then reflect the ball linear velocity component to w.r.t the wall normal and change the axis sign in the component
                // for simplicity i will assume every wall is a cube and oriented in the same directions as the world axis
                // decrease ball velocity by some percentage to act as absorbing kinetic energy
                vec3 normal = resolveWallNormal(wall->wallType);

                // cout << normal.x << " " << normal.y << " " << normal.z << "\n";

                MovementComponent *ballMovement = AOwner->getComponent<MovementComponent>();

                float velocity = ballMovement->current_velocity;
                vec3 normalizedVelocity = ballMovement->forward;

                vec3 reflectionVec = normalizedVelocity - 2.0f * glm::dot(normal, normalizedVelocity) * normal;
                if (glm::length(reflectionVec) != 0)
                    reflectionVec = glm::normalize(reflectionVec);

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
                vec3 normal = resolveWallNormal(wall->wallType);
                MovementComponent *carMovement = AOwner->getComponent<MovementComponent>();

                // carMovement->current_velocity = 0;
                carMovement->collidedWallNormal = normal;
                // carMovement->stopMovingOneFrame = true;
            }
        }

        void VarCheckPossibleGoal(RigidBodyComponent *ball, RigidBodyComponent *goal)
        {
            if (ball->tag == Tag::WALL)
                std::swap(ball, goal);

            bool applyTotalCollision = true;
            if (isCollided(ball, goal, applyTotalCollision))
            {
                // TODO: we must do something to declare winning
                std::cout << "YAY" << std::endl;
            }
        }

        vec3 resolveWallNormal(WallType wallType)
        {
            switch (wallType)
            {
            case WallType::LEFT:
                return vec3(1, 0, 0);
            case WallType::RIGHT:
                return vec3(-1, 0, 0);
            case WallType::TOP:
                return vec3(0, -1, 0);
            case WallType::DOWN:
                return vec3(0, 1, 0);
            case WallType::FRONT:
                return vec3(0, 0, 1);
            case WallType::BACK:
                return vec3(0, 0, -1);
            default:
                return vec3(0.0f, 0.0f, 0.0f);
            }
        }

    public:
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
            for (int i = 0; i < rigidBodies.size(); i++)
            {
                bool iIsBall = rigidBodies[i]->tag == BALL;
                bool iIsCar = rigidBodies[i]->tag == CAR;
                bool iIsWall = rigidBodies[i]->tag == WALL;
                bool iIsGround = rigidBodies[i]->tag == GROUND;
                bool iIsGoal = rigidBodies[i]->tag == GOAL;
                bool iIsObstacle = rigidBodies[i]->tag == NONE; // TODO: change this to obstacle
                bool iIsBomb = rigidBodies[i]->tag == BOMB;

                for (int j = i + 1; j < rigidBodies.size(); j++)
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
            for (int i = 0; i < rigidBodies.size(); i++)
            {
                bool iIsBall = rigidBodies[i]->tag == BALL;
                bool iIsCar = rigidBodies[i]->tag == CAR;
                bool iIsWall = rigidBodies[i]->tag == WALL;
                bool iIsGround = rigidBodies[i]->tag == GROUND;
                bool iIsGoal = rigidBodies[i]->tag == GOAL;
                bool iIsObstacle = rigidBodies[i]->tag == NONE; // TODO: change this to obstacle
                bool iIsBomb = rigidBodies[i]->tag == BOMB;

                for (int j = i + 1; j < rigidBodies.size(); j++)
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
                            carHitsBombCheck = true;
                    if (isBomb && isBall)
                        if (BallHitsBomb(rigidBodies[i], rigidBodies[j]))
                            ballHitsBombCheck = true;
                }
            }
            return (carHitsBombCheck || ballHitsBombCheck);
        }
    };

}