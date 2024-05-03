#include "camera.hpp"
#include "../ecs/entity.hpp"
#include "../components/movement.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
using std::cout;
using glm::vec3, glm::mat4;
namespace our
{
    vec3 getTransitionComponent(mat4 translationMatrix)
    {
        return vec3(translationMatrix[3][0], translationMatrix[3][1], translationMatrix[3][2]);
    }

    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json &data)
    {

        if (!data.is_object())
            return;
            
        std::string cameraTypeStr = data.value("cameraType", "perspective");
        if (cameraTypeStr == "orthographic")
        {
            cameraType = CameraType::ORTHOGRAPHIC;
        }
        else
        {
            cameraType = CameraType::PERSPECTIVE;
        }
        near = data.value("near", 0.01f);
        far = data.value("far", 100.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);
        distance = data.value("distance", 2.0f);
        height = data.value("height", 2.0f);
        followOwner = data.value("followOwner", false);
    }

    glm::mat4 CameraComponent::getNormalModeViewMatrix() 
    {
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();

        glm::vec3 eye = glm::vec4(8., 2., -10., 1.); // as this is a point which is camera center so w = 1

        glm::vec3 center = glm::vec4(10., 1., -10., 1.); // as this is a point which is where camera looks center so w = 1

        glm::vec3 up = glm::vec4(0., 1., 0., 0.); // as this is a vector which is camera up so w = 0

        current_position = eye;
        current_lookat = center;
        current_up = up;

        glm::mat4 lookAtMatrix = glm::lookAt(eye, center, up);

        return lookAtMatrix;
    }

    glm::mat4 CameraComponent::getFollowModeViewMatrix() 
    {
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();

        
        const glm::vec3 lookAtThis = getTransitionComponent(M);

        glm::vec3 cameraLocation = lookAtThis;

        MovementComponent* movementComponent = owner->getComponent<MovementComponent>();
        if (movementComponent != nullptr)
        {
            glm::vec3 direction = movementComponent->getMovementDirection(M) * -1.0f;
            cameraLocation = lookAtThis + direction * this->distance;
            cameraLocation.y = this->height;
        }


        glm::vec3 eye = cameraLocation; // as this is a point which is camera center so w = 1

        glm::vec3 center = lookAtThis; // as this is a point which is where camera looks center so w = 1

        glm::vec3 up = glm::vec4(0., 1., 0., 0.); // as this is a vector which is camera up so w = 0
        

        current_position = eye;
        current_lookat = center;
        current_up = up;


        glm::mat4 lookAtMatrix = glm::lookAt(eye, center, up);

        return lookAtMatrix;
    }


    // Creates and returns the camera view matrix
    glm::mat4 CameraComponent::getViewMatrix() 
    {
        if (!this->followOwner)
        {
            return this->getNormalModeViewMatrix();
        }
        else
        {
            return this->getFollowModeViewMatrix();
        }
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const
    {
        //  NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        //  It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        //  Left and Right are the same but after being multiplied by the aspect ratio
        //  For the perspective camera, you can use glm::perspective

        // assuming viewPortSize will be something line 16,9 => 16 / 9 (width / height)

        float aspectRatio = (float)viewportSize.x / viewportSize.y;

        float width = orthoHeight * aspectRatio;
        float x = width / 2;
        float y = orthoHeight / 2;
        if (cameraType == CameraType::ORTHOGRAPHIC)
        {
            return glm::ortho(-x, x, -y, y, this->near, this->far);
        }
        else
        {
            return glm::perspective(this->fovY, aspectRatio, this->near, this->far);
        }
    }
}