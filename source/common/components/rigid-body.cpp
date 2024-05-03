#include "rigid-body.hpp"
#include <glm/glm.hpp>
#include <json/json.hpp>
#include <string>


namespace our
{
    void RigidBodyComponent::deserialize(const nlohmann::json& data)
    {   
        if(!data.is_object()) return;
        
        string t = data.value("tag", "none");
        string w = data.value("wallType", "none");

        this->tag = Tag::NONE;
        this->wallType = WallType::NOWALL;



        if (t == "ball") this->tag = Tag::BALL;
        if (t == "goal") this->tag = Tag::GOAL;
        if (t == "wall") this->tag = Tag::WALL;
        if (t == "car") this->tag = Tag::CAR;
        if (t == "obstacle") this->tag = Tag::OBSTACLE;
  
        if (tag == Tag::WALL)
        {
            if (w == "left") this->wallType = WallType::LEFT;
            if (w == "right") this->wallType = WallType::RIGHT;
            if (w == "top") this->wallType = WallType::TOP;
            if (w == "down") this->wallType = WallType::DOWN;
            if (w == "front") this->wallType = WallType::FRONT;
            if (w == "back") this->wallType = WallType::BACK;
        }

        float min_x = data.value("min_x", -1.0);
        float max_x = data.value("max_x", 1.0);
        float min_y = data.value("min_y", -1.0);
        float max_y = data.value("max_y", 1.0);
        float min_z = data.value("min_z", -1.0);
        float max_z = data.value("max_z", 1.0);

        boundingBox = {
            // lower square
            vec3(min_x, min_y, min_z),
            vec3(max_x, min_y, min_z),
            vec3(max_x, min_y, max_z),
            vec3(min_x, min_y, max_z),

            // upper square
            vec3(min_x, max_y, min_z),
            vec3(max_x, max_y, min_z),
            vec3(max_x, max_y, max_z),
            vec3(min_x, max_y, max_z),
        };
    }
}