#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../components/player-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{
    class PlayerControllerSystem {
        Application* app; // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
        }

        void handleKeyboard(MovementComponent* movement, PlayerController* playerController){
            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth
            Entity* car = movement->getOwner();

            if(app->getKeyboard().isPressed(GLFW_KEY_W)) 
                movement->adjustSpeed(playerController->speedupFactor);

            if(app->getKeyboard().isPressed(GLFW_KEY_S)) 
                movement->adjustSpeed(playerController->speedupFactor * -1);
            
            float angle = playerController->rotationSensitivity;
            // A & D moves the player left or right 
            if(app->getKeyboard().isPressed(GLFW_KEY_A)) 
            {
                car->localTransform.rotation.y += movement->getRotationAngle();
            }

            if(app->getKeyboard().isPressed(GLFW_KEY_D)) {
                car->localTransform.rotation.y -= movement->getRotationAngle();
            }
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent 
        void update(World* world, float deltaTime) {
            // First of all, we search for an entity containing Player Contoller
            // As soon as we find one, we break
            
            PlayerController *controller = nullptr;
            for(auto entity : world->getEntities()){
                controller = entity->getComponent<PlayerController>();
                if(controller) break;
            }
            

            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if(!(controller)) return;


            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity* entity = controller->getOwner();
            MovementComponent* movement = entity->getComponent<MovementComponent>();


            // We get a reference to the entity's position and rotation
            glm::vec3& position = entity->localTransform.position;
            glm::vec3& rotation = entity->localTransform.rotation;
            
            handleKeyboard(movement, controller);
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit(){
            if(mouse_locked) {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }

    };

}
