#include "movement.hpp"
#include "glm/glm.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
#include <glm/gtx/euler_angles.hpp>

namespace our {
    // Reads linearVelocity & angularVelocity from the given json object
    void MovementComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        glm::mat4 initialTransformation = glm::inverse(getOwner()->getLocalToWorldMatrix());

        std::string movementTypeStr = data.value("movementType", "normal");
        if (movementTypeStr == "normal") movementType = Movement_Type::NORMAL;
        if (movementTypeStr == "fixed_direction") movementType = Movement_Type::FIXED_DIRECTION;
        if (movementTypeStr == "fixed_rotation") movementType = Movement_Type::FIXED_ROTATION;

        forward = initialTransformation * glm::vec4(data.value("forward", forward), 0.0f);
        current_velocity = data.value("initial_speed", 0.0f);
        min_velocity = -1 * data.value("max_negative_speed", -8.0f);
        max_velocity = data.value("max_positive_speed", 16.0f);
        slowdownFactor = data.value("linear_slowdown_factor", 4.0f);

        
        canRoll = data.value("canRoll", false);
        angular_velocity = data.value("initial_angular_velocity", vec3(0.0f, 0.0f, 0.0f));
        max_angular_velocity = data.value("max_angular_velocity", 6.0f);
        angularSlowdownFactor = data.value("angular_slowdown_factor", 8.0f);

        stopMovingOneFrame = false;
        collidedWallNormal = vec3(0.0f, 0.0f, 0.0f);
        
    }
} //