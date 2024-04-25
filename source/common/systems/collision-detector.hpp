#include "../components/rigid-body.hpp"
#include "../components/movement.hpp"
#include "../ecs/world.hpp"
#include <unordered_set>
#include <iostream>

using glm::vec4, glm::vec3, glm::vec4, glm::mat4, glm::distance;
using std::unordered_set, std::vector, std::max, std::min;

using std::cout;
namespace our
{
    class CollisionSystem
    {
        vec3 getTransitionComponent(mat4 translationMatrix)
        {
            return vec3(translationMatrix[3][0], translationMatrix[3][1], translationMatrix[3][2]);
        }

        void moveInLocalSpace(Entity* A,float force, vec3 direction, mat4 transMatrix)
        {
            vec3 worldMovement = direction * force;
            vec3 localMovement = transMatrix * vec4(worldMovement, 0.0);
            A->getComponent<MovementComponent>()->linearVelocity += localMovement;
        }

        void dontMoveCamera(Entity* camera)
        {
            CameraComponent* cameraComp = camera->getComponent<CameraComponent>();
            cameraComp->stopMovingOneFram = true;
        }


    public:

        bool cubesCollision(const vector<vec3>& a_vertices, const vector<vec3>& b_vertices)
        {
            vec3 a_maxCoordinates = vec3(INT32_MIN,INT32_MIN,INT32_MIN);
            vec3 a_minCoordinates = vec3(INT32_MAX,INT32_MAX,INT32_MAX);

            vec3 b_maxCoordinates = vec3(INT32_MIN,INT32_MIN,INT32_MIN);
            vec3 b_minCoordinates = vec3(INT32_MAX,INT32_MAX,INT32_MAX);

            for (vec3 point : a_vertices) {
                a_maxCoordinates.x = max(a_maxCoordinates.x, point.x), a_maxCoordinates.y = max(a_maxCoordinates.y, point.y), a_maxCoordinates.z = max(a_maxCoordinates.z, point.z);
                a_minCoordinates.x = min(a_minCoordinates.x, point.x), a_minCoordinates.y = min(a_minCoordinates.y, point.y), a_minCoordinates.z = min(a_minCoordinates.z, point.z);
            }

            for (vec3 point : b_vertices) {
                b_maxCoordinates.x = max(b_maxCoordinates.x, point.x), b_maxCoordinates.y = max(b_maxCoordinates.y, point.y), b_maxCoordinates.z = max(b_maxCoordinates.z, point.z);
                b_minCoordinates.x = min(b_minCoordinates.x, point.x), b_minCoordinates.y = min(b_minCoordinates.y, point.y), b_minCoordinates.z = min(b_minCoordinates.z, point.z);
            }

            cout << a_minCoordinates.x << " " << a_minCoordinates.z << std::endl;
            cout << a_maxCoordinates.x << " " << a_maxCoordinates.z << std::endl;
            
            cout << b_minCoordinates.x << " " << b_minCoordinates.z << std::endl;
            cout << b_maxCoordinates.x << " " << b_maxCoordinates.z << std::endl;


            bool overlapX = a_minCoordinates.x >= b_minCoordinates.x && a_minCoordinates.x <= b_maxCoordinates.x;
            overlapX |= b_minCoordinates.x >= a_minCoordinates.x && b_minCoordinates.x <= a_maxCoordinates.x;

            
            bool overlapY = a_minCoordinates.y >= b_minCoordinates.y && a_minCoordinates.y <= b_maxCoordinates.y;
            overlapY |= b_minCoordinates.y >= a_minCoordinates.y && b_minCoordinates.y <= a_maxCoordinates.y;

            
            bool overlapZ = a_minCoordinates.z >= b_minCoordinates.z && a_minCoordinates.z <= b_maxCoordinates.z;
            overlapZ |= b_minCoordinates.z >= a_minCoordinates.z && b_minCoordinates.z <= a_maxCoordinates.z;

            return overlapX && overlapY && overlapZ;
        }



        void isCollided(RigidBodyComponent* A, RigidBodyComponent* B)
        {
            if (A->tag == GROUND || B->tag == GROUND) return;
            Entity* AOwner = A->getOwner()->parent;
            Entity* BOwner = B->getOwner()->parent;

            mat4 a_local_to_world = AOwner->getLocalToWorldMatrix();
            mat4 b_local_to_world = BOwner->getLocalToWorldMatrix();

            vec3 center_a = getTransitionComponent(a_local_to_world);
            vec3 center_b = getTransitionComponent(b_local_to_world);

            bool isCollided = false;

            if (A->bodyType == CUBE && B->bodyType == CUBE)
            {
                const vector<vec3> AVertices = A->translateToWorld(a_local_to_world);
                const vector<vec3> BVertices = B->translateToWorld(b_local_to_world);
                isCollided = cubesCollision(AVertices, BVertices);
            }
            else 
            {
                cout << distance(center_a, center_b) << std::endl;
                isCollided = distance(center_a, center_b) < 2;
            }

            if(isCollided)
            {
                vec3 normal = glm::normalize(center_b - center_a);
                if (!A->isStatic) // move with the negative of the normal, but the normal is in the world space so we need to convert it to a local space
                {
                    moveInLocalSpace(AOwner, 6, -normal, glm::inverse(a_local_to_world));
                }
                else if (A->tag == CAR){
                    // don't move the camera
                    dontMoveCamera(AOwner->parent);
                }

                if (!B->isStatic)
                {
                    moveInLocalSpace(BOwner, 6, normal, glm::inverse(b_local_to_world));
                }
                else if (A->tag == CAR){
                    // don't move the camera
                    dontMoveCamera(BOwner->parent);
                }
            }
            
        }




        void checkForCollisions(World* world)
        {
            unordered_set<Entity*> entities = world->getEntities();
            vector<RigidBodyComponent*> rigidBodies;
            for (Entity* entity : entities)
            {
                RigidBodyComponent* rigidBody = entity->getComponent<RigidBodyComponent>();
                if (rigidBody == nullptr) continue;
                rigidBodies.push_back(rigidBody);
            }
            for (int i = 0; i < rigidBodies.size(); i++)
            {
                for (int j = i + 1; j < rigidBodies.size(); j++)
                {
                    isCollided(rigidBodies[i], rigidBodies[j]);
                }
            }
        }
    };
    
}