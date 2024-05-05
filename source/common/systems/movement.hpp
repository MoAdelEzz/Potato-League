#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../components/ball-component.hpp"
#include "../ecs/transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "collision-detector.hpp"

using glm::vec3;
using std::max;

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class MovementSystem
    {
    private:
        void applyAccelration(MovementComponent *movementComponent, float deltaTime)
        {
            movementComponent->decreaseSpeed(deltaTime);
        }

    public:
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime)
        {
            // For each entity in the world
            for (Entity *entity : world->getEntities())
            {
                // Get the movement component if it exists
                MovementComponent *movement = entity->getComponent<MovementComponent>();
                // If the movement component exists
                if (movement && !movement->stopMovingOneFrame)
                {
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    applyAccelration(movement, deltaTime);
                    movement->adjustSpeed(0.0f);

                    // TODO: move this inside movement
                    if (movement->collidedWallNormal != vec3(0.0, 0.0, 0.0))
                    {
                        vec3 carDirection = entity->localTransform.convertToLocalSpace(movement->forward);
                        carDirection *= movement->current_velocity > 0 ? 1 : -1;

                        if (glm::dot(carDirection, movement->collidedWallNormal) < 0)
                            continue;

                        movement->collidedWallNormal = vec3(0.0, 0.0, 0.0);
                    }

                    movement->updateAngle(deltaTime);
                    movement->updateJumpState(deltaTime);

                    if (!movement->directedMovementMode)
                    {
                        float speed =  movement->current_velocity * (movement->boosting ? 1.5 : 1);
                        entity->localTransform.applyLinearVelocity(movement->forward, deltaTime * speed);
                    }
                    else
                    {
                        glm::vec3 target_in_world = movement->targetPointInWorldSpace;
                        glm::vec3 source_in_world = entity->getLocalToWorldCenter();
                        entity->localTransform.moveTowards(target_in_world, source_in_world, deltaTime * movement->max_velocity);
                    }

                    if (entity->getComponent<BallComponent>() == nullptr)
                        entity->localTransform.applyAngularVelocity(movement->angular_velocity);
                }
                else if (movement)
                {
                    movement->stopMovingOneFrame = false;
                }
            }
        }
    };

}
