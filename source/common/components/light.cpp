#include "light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        lightType = data.value("lightType", 0);
        direction = data.value("direction", glm::vec3(0, 0, 0));
        ambientColor = data.value("ambientColor", glm::vec3(0, 0, 0));
        diffuseColor = data.value("diffuseColor", glm::vec3(0, 0, 0));
        specularColor = data.value("specularColor", glm::vec3(0, 0, 0));
        attenuation = data.value("attenuation", glm::vec3(1, 1, 1));
        coneAngles = data.value("coneAngles", glm::vec2(0, 0));
    }
}