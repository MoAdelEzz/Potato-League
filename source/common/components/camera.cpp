#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
using std::cout;

namespace our
{
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
    }

    // Creates and returns the camera view matrix
    glm::mat4 CameraComponent::getViewMatrix() const
    {
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();
        // TODO: (Req 8) Complete this function
        // HINT:
        //  In the camera space:
        //  - eye is the origin (0,0,0)
        //  - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        //  - up is the direction (0,1,0)
        //  but to use glm::lookAt, we need eye, center and up in the world state.
        //  Since M (see above) transforms from the camera to thw world space, you can use M to compute:
        //  - the eye position which is the point (0,0,0) but after being transformed by M
        //  - the center position which is the point (0,0,-1) but after being transformed by M
        //  - the up direction which is the vector (0,1,0) but after being transformed by M
        //  then you can use glm::lookAt

        // here we build a local transformation matrix of the camera
        // eye is the center of the camera at origin
        // center is where are we looking
        // up is where is the up vector of my camera

        glm::vec3 eye = M * glm::vec4(0., 0., 0., 1.); // as this is a point which is camera center so w = 1

        glm::vec3 center = M * glm::vec4(0., 0., -1., 1.); // as this is a point which is where camera looks center so w = 1

        glm::vec3 up = M * glm::vec4(0., 1., 0., 0.); // as this is a vector which is camera up so w = 0

        glm::mat4 lookAtMatrix = glm::lookAt(eye, center, up);


        return lookAtMatrix;
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const
    {
        // TODO: (Req 8) Wrtie this function
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