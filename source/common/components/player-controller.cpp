#include "player-controller.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    // Reads sensitivities & speedupFactor from the given json object
    void PlayerController::deserialize(const nlohmann::json& data){
        // if(!data.is_object()) return;
        // TODO: just make it simple for now
        rotationSensitivity = data.value("rotationSensitivity", rotationSensitivity);
        speedupFactor = data.value("speedupFactor", speedupFactor);
    }
}