#pragma once

#include "../ecs/entity.hpp"
#include "../ecs/component.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtx/euler_angles.hpp>

#define MIN_SPEED_FOR_ROTATION 2
#define ROTATION_CONSTANT 0.002f
#define ROTATION_SENSITIVITY 0.05f

using glm::vec3, glm::vec4, glm::mat4;

namespace our
{

    // This component denotes that the MovementSystem will move the owning entity by a certain linear and angular velocity.
    // This component is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/systems/movement.hpp"
    // For a more complex example of how to use the ECS framework, see "free-camera-controller.hpp"

    enum Movement_Type
    {
        NORMAL,
        FIXED_DIRECTION,
        FIXED_ROTATION
    };

    class MovementComponent : public Component
    {

    public:
        Movement_Type movementType;

        // linear velocity
        glm::vec3 forward = {0, 0, -1};
        float current_velocity = 0.0f;
        float min_velocity = -8.f;
        float max_velocity = 32.f;
        float slowdownFactor = 8.0f;
        bool constant_movement_x = false;
        bool constant_movement_y = false;
        bool constant_movement_z = false;

        glm::vec3 final_value = glm::vec3(0.0, 0.0, 0.0);

        // angular rotation
        bool canRoll = false;
        vec3 current_angle = {0.0f, 0.0f, 0.0f};
        glm::vec3 angular_velocity = {0, 0, 0};
        float max_angular_velocity = 6.0f;
        float angularSlowdownFactor = 8.0f;

        vec3 lastWallNormal = vec3(0, 0, 0);
        bool stopMovingOneFrame = false;
        glm::vec3 collidedWallNormal = glm::vec3(0.0, 0.0, 0.0);

        // The ID of this component type is "Movement"
        static std::string getID() { return "Movement"; }

        glm::vec3 getMovementDirection(glm::mat4 M)
        {
            return M * vec4(forward, 0.0f);
        }

        glm::vec3 getLookAtPoint(glm::mat4 M)
        {
            return M * vec4(forward, 1.0f);
        }

        void adjustSpeed(float factor)
        {
            current_velocity += factor;
            if (current_velocity >= max_velocity)
                current_velocity = max_velocity;
            if (current_velocity <= min_velocity)
                current_velocity = min_velocity;
        }

        void roll()
        {
            angular_velocity.x = 0.8f * current_velocity;
            angular_velocity.x = std::min(angular_velocity.x, max_angular_velocity);
        }

        void updateAngle(float deltatime)
        {
            current_angle += angular_velocity * (current_velocity > 0 ? 1.0f : current_velocity == 0.0f ? 0
                                                                                                        : -1.0f) *
                             deltatime;
            if (current_angle.x > 360)
                current_angle.x -= 360;
            if (current_angle.y > 360)
                current_angle.y -= 360;
            if (current_angle.z > 360)
                current_angle.z -= 360;
        }

        bool isMoving() { return current_velocity > MIN_SPEED_FOR_ROTATION; }

        float getRotationAngle()
        {
            return ROTATION_CONSTANT * current_velocity;
        }

        void setForward(glm::vec3 forw)
        {
            if (glm::length(forw) > 0)
                forw = glm::normalize(forw);
            forward = forw;
        }

        void decreaseSpeed(float deltaTime)
        {
            if (current_velocity == 0)
                return;

            float absSpeed = abs(current_velocity);

            int sign = current_velocity > 0 ? 1 : -1;

            absSpeed -= slowdownFactor * deltaTime;

            if (absSpeed < 0)
                absSpeed = 0;

            current_velocity = absSpeed * sign;
            if (canRoll)
                roll();
        }

        // Reads linearVelocity & angularVelocity from the given json object
        void deserialize(const nlohmann::json &data) override;
    };

}