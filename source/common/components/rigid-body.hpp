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

            vector<vec3> vertices;
            vec3 normal;
            float radius; // for sphere

            float mass;
            float density;

            bool isStatic;


            Tag tag;
            WallType wallType; // for walls

        RigidBodyComponent(){}
        ~RigidBodyComponent(){}

        vector<vec3> translateToWorld(glm::mat4 translationMatrix)
        {
            vector<vec3> translatedVertices;
            for (vec3 vertex : vertices)
            {
                translatedVertices.push_back( translationMatrix * vec4(vertex, 1.0f) );
            }
            return translatedVertices;
        }

        static string getID() {return "Rigid Body";}
        void deserialize(const nlohmann::json& data) override;
    };
}