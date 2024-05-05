#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/player-controller.hpp>
#include <systems/collision-detector.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include "./menu-state.hpp"

#include "../common/ecs/transform.hpp"
#include <../common/systems/sound/sound.hpp>

#include <unordered_set>
#include <iostream>

#define level_minutes 0
#define level_seconds 15

// This state shows how to use the ECS framework and deserialization.
class Level3state : public our::State
{

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::PlayerControllerSystem playerController;
    our::MovementSystem movementSystem;
    our::CollisionSystem collisionSystem;
    bool bombExplodes = false;

    bool timeUp = false;

    int minutes = level_minutes;
    int seconds = level_seconds;
    int lives = 3;
    bool carSoundCheck = false;

    time_t previousTime;
    time_t currentTime;

    int countDownTime = 5;
    bool countDownState = true;

    bool hurryUp = false;

    AudioPlayer *soundSystem = AudioPlayer ::getInstance();

    our::TexturedMaterial *timeMaterial;
    float time1;
    our::Mesh *timerRectangle;

    void onInitialize() override
    {
        soundSystem->stopAllSounds();

        timerRectangle = new our::Mesh({
                                           {{0.0f + 0.29, 0.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
                                           {{4 * 0.108f + 0.29, 0.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
                                           {{4 * 0.108f + 0.29, 4 * 0.0348f, 0.0f}, {255, 255, 255, 255}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                                           {{0.0f + 0.29, 4 * 0.0348f, 0.0f}, {255, 255, 255, 255}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                                       },
                                       {
                                           0,
                                           1,
                                           2,
                                           2,
                                           3,
                                           0,
                                       });
        carSoundCheck = false;

        time1 = 0;
        timeMaterial = new our::TexturedMaterial();
        // Here, we load the shader that will be used to draw the background
        timeMaterial->shader = new our::ShaderProgram();
        timeMaterial->shader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
        timeMaterial->shader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
        timeMaterial->shader->link();
        // Then we load the menu texture
        timeMaterial->texture = our::texture_utils::loadImage("assets/textures/timer.png");
        // Initially, the menu material will be black, then it will fade in
        timeMaterial->tint = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

        previousTime = time(NULL);
        countDownState = true;
        timeUp = false;
        countDownTime = 5;
        lives = 3;
        minutes = level_minutes;
        seconds = level_seconds;
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig(3)["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets"))
        {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world"))
        {
            world.deserialize(config["world"]);
        }

        // TODO: remove this if not used
        // world.focusCamera();

        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        playerController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);

        soundSystem->playSound("countdown");
    }

    void decreaseTime()
    {

        int totalTime = seconds + minutes * 60;
        totalTime--;

        if (totalTime == 0)
            timeUp = true;

        seconds = totalTime % 60;
        minutes = totalTime / 60;
    }

    void handleTimer()
    {
        std::string minutesString;
        std::string secondsString;

        if (minutes <= 9)
        {
            minutesString = "0";
        }
        minutesString += std::to_string(minutes);

        if (seconds <= 9)
        {
            secondsString = "0";
        }

        secondsString += std::to_string(seconds);

        getApp()->printTextCenter(minutesString + " : " + secondsString, 32, 3);
    }

    void handleReset()
    {
        unordered_set<our::Entity *> entities = world.getEntities();
        for (our::Entity *entity : entities)
        {
            entity->localTransform.position = entity->localTransform.initialPositionNew;
            entity->localTransform.rotation = entity->localTransform.initialRotationNew;
            entity->localTransform.scale = entity->localTransform.initialScaleNew;
        }

        vector<our::MovementComponent *> movements;
        for (our::Entity *entity : entities)
        {
            our::MovementComponent *movement = entity->getComponent<our::MovementComponent>();
            if (movement == nullptr)
                continue;
            movements.push_back(movement);
        }
        for (unsigned int i = 0; i < movements.size(); i++)
        {
            movements[i]->current_velocity = 0;
        }
    }
    void handleCountDown()
    {
        if (countDownTime > 0)
        {
            countDownTime--;
        }
        else
        {
            countDownState = false;
        }
    }

    void handleTime()
    {
        currentTime = time(NULL);
        if (currentTime - previousTime >= 1)
        {
            previousTime = time(NULL);
            if (!countDownState)
                decreaseTime();
            else
                handleCountDown();
        }
    }

    void timerDraw(float deltaTime)
    {
        glDisable(GL_BLEND);
        glm::ivec2 size = getApp()->getFrameBufferSize();
        glViewport(0, 0, size.x, size.y);

        glm::mat4 VP = glm::ortho(0.0f, (float)size.x, (float)size.y, 0.0f, 1.0f, -1.0f);
        glm::mat4 M = glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

        time1 += (float)deltaTime;
        timeMaterial->tint = glm::vec4(glm::smoothstep(0.00f, 2.00f, time1));
        timeMaterial->setup();
        timeMaterial->shader->set("transform", VP * M);
        timerRectangle->draw();
    }

    void handleBombMovement()
    {
        vector<our::RigidBodyComponent *> bombBodies;
        vector<our::MovementComponent *> bombMoves;

        for (our::Entity *entity : world.getEntities())
        {
            our::RigidBodyComponent *rigidBody = entity->getComponent<our::RigidBodyComponent>();
            our::MovementComponent *movement = entity->getComponent<our::MovementComponent>();

            if (rigidBody == nullptr || movement == nullptr)
                continue;
            bombBodies.push_back(rigidBody);
            bombMoves.push_back(movement);
        }
        glm::vec3 carLocation;
        for (unsigned int i = 0; i < bombBodies.size(); i++)
        {
            if (bombBodies[i]->tag == our::Tag::CAR)
            {
                carLocation = bombMoves[i]->getCurrentPositionInWorld();
            }
        }
        for (unsigned int i = 0; i < bombBodies.size(); i++)
        {
            if (bombBodies[i]->tag == our::Tag::BOMB)
            {
                bombMoves[i]->targetPointInWorldSpace = carLocation;
                bombMoves[i]->targetPointInWorldSpace.y = 1.0f; // MOA: to make the player dodge bombs by jumping
            }
        }
    }

    void handleBombExplodes()
    {
        lives--;
        bombExplodes = false;
        seconds = level_seconds;
        minutes = level_minutes;
        soundSystem->playSound("bomb");
        handleReset();
    }

    void printLives()
    {
        getApp()->printTextLeft("Lives   " + std::to_string(lives), 32, 3);
    }

    void handleCarSoundStop()
    {
        vector<our::RigidBodyComponent *> rigidBodies;
        vector<our::MovementComponent *> movementBodies;
        vector<our::PlayerController *> players;

        for (our::Entity *entity : world.getEntities())
        {
            our::RigidBodyComponent *rigidBody = entity->getComponent<our::RigidBodyComponent>();
            our::MovementComponent *movement = entity->getComponent<our::MovementComponent>();
            our::PlayerController *player = entity->getComponent<our::PlayerController>();

            if (rigidBody == nullptr || movement == nullptr || player == nullptr)
                continue;
            rigidBodies.push_back(rigidBody);
            movementBodies.push_back(movement);
        }
        for (unsigned int i = 0; i < movementBodies.size(); i++)
        {
            if (rigidBodies[i]->tag == our::Tag::CAR)
            {
                if (movementBodies[i]->current_velocity <= 0.01)
                {
                    soundSystem->stopSound("car");
                    carSoundCheck = false;
                }
            }
        }
    }
    void onDraw(double deltaTime) override
    {

        handleTime();
        timerDraw(deltaTime);
        handleTimer();
        printLives();

        if (!countDownState)
        {
            collisionSystem.checkForCollisions(&world);
            bombExplodes = collisionSystem.checkForBombCollision(&world);
            playerController.update(&world, (float)deltaTime);
            movementSystem.update(&world, (float)deltaTime);
            cameraController.update(&world, (float)deltaTime);
            handleBombMovement();
        }

        renderer.render(&world);

        auto &keyboard = getApp()->getKeyboard();

        if (keyboard.justPressed(GLFW_KEY_ESCAPE))
        {
            // If the escape  key is pressed in this frame, go to the menu state
            soundSystem->stopAllSounds();
            getApp()->changeState(Menustate::getStateName_s());
        }
        else if (keyboard.isPressed(GLFW_KEY_W))
        {
            if (!carSoundCheck && !countDownState)
            {
                soundSystem->playSound("car");
                carSoundCheck = true;
            }
        }
        else if (keyboard.isPressed(GLFW_KEY_S))
        {
            if (!carSoundCheck && !countDownState)
            {
                soundSystem->playSound("car");
                carSoundCheck = true;
            }
        }
        else
        {
            if (carSoundCheck && !countDownState)
            {
                soundSystem->stopSound("car");
                // soundSystem->playSound("carstop");
                carSoundCheck = false;
            }
        }

        if (bombExplodes)
        {
            soundSystem->stopSound("hurryup");
            handleBombExplodes();
        }

        if (lives <= 0)
        {
            countDownState = true;
            soundSystem->stopAllSounds();
            soundSystem->playSound("gameover");
            getApp()->changeState("lose-state");
        }

        if (timeUp)
        {
            countDownState = true;
            soundSystem->stopAllSounds();
            soundSystem->playSound("win");
            getApp()->changeState("win-state");
        }

        if (minutes == 0 && seconds <= 3 && !hurryUp)
        {
            hurryUp = true;
            soundSystem->playSound("hurryup");
        }
        else if (seconds > 2)
        {
            hurryUp = false;
        }
    }

    void onDestroy() override
    {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        playerController.exit();
        // Clear the world
        world.clear();
        countDownState = true;
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }

public:
    static std::string getStateName_s()
    {
        return "level3";
    }
    std::string getStateName()
    {
        return "level3";
    }
};