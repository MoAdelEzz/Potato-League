#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

#define xAxis glm::vec3(1., 0., 0.)
#define yAxis glm::vec3(0., 1., 0.)
#define zAxis glm::vec3(0., 0., 1.)

namespace our
{

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const
    {
        // TODO: (Req 3) Write this function

        glm::mat4 identityMatrix = glm::mat4(1.0);

        glm::mat4 scalingMatrix = glm::scale(identityMatrix, this->scale);

        glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);

        glm::mat4 translationMatrix = glm::translate(identityMatrix, this->position);

        glm::mat4 answer = translationMatrix * rotationMatrix * scalingMatrix;

        return answer;
    }

    glm::vec3 Transform::convertToLocalSpace(glm::vec3 vector)
    {
        glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        glm::vec3 rotatedVector = rotationMatrix * glm::vec4(vector, 0.0f);

        return rotatedVector;
    }

    void Transform::applyLinearVelocity(glm::vec3 forward, float velocity)
    {
        glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        glm::vec3 rotatedForward = rotationMatrix * glm::vec4(forward, 0.0f);

        position += rotatedForward * velocity;
    }

    // point is in local space
    void Transform::moveTowards(glm::vec3 destination, glm::vec3 source, float velocity)
    {
        glm::vec3 direction = glm::length(destination - source) > 0.1 ? glm::normalize(destination - source) : glm::vec3(0.0, 0.0, 0.0);
        position += direction * velocity;
    }

    void Transform::applyAngularVelocity(glm::vec3 velocity)
    {

        rotation += glm::radians(velocity);
    }

    // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json &data)
    {
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale = data.value("scale", scale);

        initialPositionNew = position;
        initialRotationNew = rotation;
        initialScaleNew = scale;
    }

}