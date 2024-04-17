#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

// TODO: remove this
#include <iostream>

#define xAxis glm::vec3(1.,0.,0.)
#define yAxis glm::vec3(0.,1.,0.)
#define zAxis glm::vec3(0.,0.,1.)

using std::cout;

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write this function
        
        glm::mat4 identityMatrix = glm::mat4(1.0);

        glm::mat4 scalingMatrix = glm::scale(identityMatrix, this->scale);

        glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
    
        glm::mat4 translationMatrix = glm::translate(identityMatrix, this->position);
    
        glm::mat4 answer = translationMatrix * rotationMatrix * scalingMatrix;

        return answer; 
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}