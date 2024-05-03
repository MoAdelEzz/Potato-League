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
        NONE = 0,
        WALL = 1,
        GROUND = 2,
        BALL = 3,
        CAR = 4,
        GOAL = 5,
        OBSTACLE = 6,
        BOMB = 7
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
            float mass = 10.0;
            float density = 10.0;
            bool isStatic = false;

            vec3 min_point = {-1, -1, -1};
            vec3 max_point = {1, 1, 1};

            vector<vec3> boundingBox = {
                    vec3(1.0, 1.0, 1.0)   , 
                    vec3(-1.0, 1.0, 1.0)  , vec3(1.0, -1.0, 1.0), vec3(1.0, 1.0, -1.0),
                    vec3(-1.0, -1.0, 1.0) , vec3(1.0, -1.0, -1.0), vec3(-1.0, 1.0, -1.0),
                    vec3(-1.0, -1.0, -1.0)
            };

            Tag tag;
            WallType wallType; // for walls

        RigidBodyComponent(){}
        ~RigidBodyComponent(){}


        vector<vec3> getBoxNormals(glm::mat4 transformation)
        {
            vector<vec3> normals;

            // bottom face
            vec3 edge_1 = boundingBox[1] - boundingBox[0];
            vec3 edge_2 = boundingBox[3] - boundingBox[0];
            normals.push_back(glm::normalize(transformation * vec4(glm::cross(edge_1, edge_2), 0.0)));

            // left face
            edge_1 = boundingBox[4] - boundingBox[0];
            edge_2 = boundingBox[1] - boundingBox[0];
            normals.push_back(glm::normalize(transformation * vec4(glm::cross(edge_1, edge_2), 0.0)));

            // front face
            edge_1 = boundingBox[5] - boundingBox[1];
            edge_2 = boundingBox[2] - boundingBox[1];
            normals.push_back(glm::normalize(transformation * vec4(glm::cross(edge_1, edge_2), 0.0)));
            
            
            return normals;
        }

        vector<float> getProjectionRange(vec3 axis, glm::mat4 transformation)
        {
            float min_projection = INT32_MAX;
            float max_projection = INT32_MIN;

            for (auto& point : boundingBox)
            {
                float projection = glm::dot(vec3(transformation * vec4(point, 1.0)), axis);
                min_projection = std::min(min_projection, projection);
                max_projection = std::max(max_projection, projection);
            }
            return {min_projection, max_projection};
        }


        vector<vec3> getBoundingBox()
        {
            return boundingBox;
        }

        static string getID() {return "Rigid Body";}
        void deserialize(const nlohmann::json& data) override;
    };
}