#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtx/euler_angles.hpp>

using std::endl, std::cout;

namespace our {

    // This component denotes that the MovementSystem will move the owning entity by a certain linear and angular velocity.
    // This component is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/systems/movement.hpp"
    // For a more complex example of how to use the ECS framework, see "free-camera-controller.hpp"
    class MovementComponent : public Component {
    public:
        glm::vec3 forward = {0, 0, -1};
        glm::vec3 right = {1, 0, 0};

        float slowdownFactor = 8.0f;

        float current_velocity = 0.0f;
        float min_velocity = -8.f;
        float max_velocity = 20.f;

        bool stopMovingOneFrame = false;

        // The ID of this component type is "Movement"
        static std::string getID() { return "Movement"; }

        glm::vec3 getMovementDirection(glm::mat4 M)
        {
            glm::vec3 normalizedVector = glm::length(forward) != 0 ? glm::normalize(forward) : glm::vec3(0.,0.,0.);
            return M * glm::vec4(normalizedVector, 0.0f);
        }

        void adjustSpeed(float factor)
        {
            current_velocity += factor;
            if (current_velocity >= max_velocity) current_velocity = max_velocity;
            if (current_velocity <= min_velocity) current_velocity = min_velocity;
        }

        void setForward(glm::vec3 forw){
            forward = forw;
        }

        void decreaseSpeed(float slowdown)
        {
            float absSpeed = abs(current_velocity);

            int sign = current_velocity > 0 ? 1 : -1;

            absSpeed -= slowdown;


            std::cout << absSpeed << std::endl;

            
            if (absSpeed < 0) absSpeed = 0;

            current_velocity = absSpeed * sign;
        }

        void turn(float angle)
        {
            float yaw = glm::radians(angle);   // Yaw angle (rotation around the y-axis)
            float pitch = glm::radians(0.0f); // Pitch angle (rotation around the x-axis)
            float roll = glm::radians(0.0f);  // Roll angle (rotation around the z-axis)

            // Create a rotation matrix using yaw, pitch, and roll
            glm::mat4 rotationMatrix = glm::yawPitchRoll(yaw, pitch, roll);

            // Apply the rotation to the vector
            forward = rotationMatrix * glm::vec4(forward, .0f);

        }

        // Reads linearVelocity & angularVelocity from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}