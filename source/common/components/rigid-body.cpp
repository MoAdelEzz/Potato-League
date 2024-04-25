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

            float length = 1.0f;
            float width = data.value("width", 1.0f);
            float height = data.value("height", 1.0f);


            vertices.push_back(vec3( width / 2, length / 2, height / 2 ));

            vertices.push_back(vec3( -width / 2, length / 2, height / 2 ));
            vertices.push_back(vec3( width / 2, -length / 2, height / 2 ));
            vertices.push_back(vec3( width / 2, length / 2, -height / 2 ));

            vertices.push_back(vec3( -width / 2, -length / 2, height / 2 ));
            vertices.push_back(vec3( width / 2, -length / 2, -height / 2 ));
            vertices.push_back(vec3( -width / 2, length / 2, -height / 2 ));

            vertices.push_back(vec3( -width / 2, -length / 2, -height / 2 ));
        }

        this->density = 10.0; // for now
        this->mass = 10.0; // for now
        this->isStatic = data.value("static", false);
        
        string t = data.value("tag", "none");
        if (t == "none") this->tag = NONE;
        if (t == "ball") this->tag = BALL;
        if (t == "goal") this->tag = GOAL;
        if (t == "wall") this->tag = WALL;
        if (t == "car") this->tag = CAR;
    }
}