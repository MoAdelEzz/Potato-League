#pragma once

#include "../ecs/component.hpp"
#include <glm/mat4x4.hpp>


using std::string;
namespace our {

    class BallComponent : public Component {
    public:
        BallComponent() {}
        
        static std::string getID() { return "Ball Component"; }

        // Reads sensitivities & speedupFactor from the given json object
        void deserialize(const nlohmann::json& data) override {};
    };

}