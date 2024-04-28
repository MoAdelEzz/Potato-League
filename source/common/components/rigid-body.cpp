#include "rigid-body.hpp"
#include <glm/glm.hpp>
#include <json/json.hpp>
#include <string>


namespace our
{
    void RigidBodyComponent::deserialize(const nlohmann::json& data)
    {   
        if(!data.is_object()) return;

        std::string type = data["bodyType"].get<std::string>();
        
        if (type == "sphere") 
        {
            this->bodyType = SPHERE;
            this->radius = data.value("radius", 10.0f);
        }
        else 
        {
            this->bodyType = CUBE;
        }

        this->density = 10.0; // for now
        this->mass = 10.0; // for now
        this->isStatic = data.value("static", false);
        
        string t = data.value("tag", "none");
        string w = data.value("wallType", "none");

        this->tag = Tag::NONE;
        this->wallType = WallType::NOWALL;


        if (t == "ball") this->tag = Tag::BALL;
        if (t == "goal") this->tag = Tag::GOAL;
        if (t == "wall") this->tag = Tag::WALL;
        if (t == "car") this->tag = Tag::CAR;

        if (tag == Tag::WALL)
        {
            if (w == "left") this->wallType = WallType::LEFT;
            if (w == "right") this->wallType = WallType::RIGHT;
            if (w == "top") this->wallType = WallType::TOP;
            if (w == "down") this->wallType = WallType::DOWN;
            if (w == "front") this->wallType = WallType::FRONT;
            if (w == "back") this->wallType = WallType::BACK;
        }
    }
}