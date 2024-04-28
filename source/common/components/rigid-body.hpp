#pragma once

#include <vector>
#include "../ecs/component.hpp"
#include "../ecs/entity.hpp"
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>


using glm::vec3, glm::vec4;
using std::string, std::vector;

namespace our
{
    enum BodyType
    {
        CUBE,
        SPHERE
    };

    enum Tag
    {
        NONE,
        WALL,
        GROUND,
        BALL,
        CAR,
        GOAL
    };


    enum WallType
    {
        NOWALL,
        LEFT, 
        RIGHT,
        TOP, 
        DOWN, 
        FRONT,
        BACK
    };

    class RigidBodyComponent : public Component
    {

        public:
            BodyType bodyType;

            vec3 normal;
            float radius; // for sphere

            float mass;
            float density;

            bool isStatic;


            Tag tag;
            WallType wallType; // for walls

        RigidBodyComponent(){}
        ~RigidBodyComponent(){}

        static string getID() {return "Rigid Body";}
        void deserialize(const nlohmann::json& data) override;
    };
}