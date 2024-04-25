#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

using std::max;
using glm::vec3;

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/movement.hpp"
    class MovementSystem {
    private:
        vec3 applyAccelration(vec3 velocity, float max_velocity, float slowDownFactor, float deltaTime)
        {
            float magVelocity = glm::length(velocity);
            vec3 unitVector = vec3(0.,0.,0.);
            if (magVelocity != 0)
                unitVector = glm::normalize(velocity);

            magVelocity -= slowDownFactor * deltaTime;
            
            if (magVelocity <= 0) 
                magVelocity = 0;
            
            if (magVelocity > max_velocity)
                magVelocity = max_velocity;


            return unitVector * magVelocity;
        }
    
    public:

        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime) {
            // For each entity in the world
            for(auto entity : world->getEntities()){
                // Get the movement component if it exists
                MovementComponent* movement = entity->getComponent<MovementComponent>();
                // If the movement component exists
                if(movement){
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    movement->linearVelocity = applyAccelration(movement->linearVelocity, movement->max_velocity, movement->slowdownFactor, deltaTime);                    

                    entity->localTransform.position += deltaTime * movement->linearVelocity;
                    entity->localTransform.rotation += deltaTime * movement->angularVelocity;
                }
            }
        }

    };

}
