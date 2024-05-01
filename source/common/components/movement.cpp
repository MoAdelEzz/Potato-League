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
        forward = initialTransformation * glm::vec4(data.value("forward", forward), 0.0f);
        canRoll = data.value("canRoll", canRoll);
    }
} //