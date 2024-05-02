import json
from movement_component import Movement
from camera_component import CameraComponent, createController
from enums import *

class JsonUtils:
    @staticmethod
    def concatenate(json_1, json_2):
        return {**json_1, **json_2}

class Entity:
    position = [0, 0, 0]
    rotation = [0, 0, 0]
    scale = [0, 0, 0]

    children = []
    components = []

    def addComponent(self, component): self.components.append(component)
    def addChild(self, child): self.children.append(child)

    def getJsonObject(self):
        json_obj = {"position": self.position}
        json_obj = JsonUtils.concatenate(json_obj, {"rotation": self.rotation})
        json_obj = JsonUtils.concatenate(json_obj, {"scale": self.scale})
        
        components_list = []
        for component in self.components:
            components_list.append(component.getJsonObject())
        if len(components_list) > 0:
            json_obj = JsonUtils.concatenate(json_obj, {"components": components_list})

        children_list = []
        for child in self.children:
            children_list.append(child.getJsonObject())
        if len(children_list) > 0:
            json_obj = JsonUtils.concatenate(json_obj, {"children": children_list})

        return json_obj

car = Entity()

camera = CameraComponent()
camera.configurePrespectiveCamera()
# camera.configreOrthographicCamera()
# camera.configureFollowMode(distance_from_the_object=2, height_above_the_object=10)
camera.configureFixedMode()

controller = createController(camera.followOwner)




car.addComponent(camera)

movement = Movement(MovementType.NORMAL_MODE)

car.addComponent(movement)



with open("output.test", "w") as json_file:
    json.dump(camera.getJsonObject(), json_file, indent=4)
    json_file.write(',')
    json_file.write('\n')

    json.dump(controller.getJsonObject(), json_file, indent=4)
    json_file.write(',')
    json_file.write('\n')

    json.dump(movement.getJsonObject(), json_file, indent=4)
    json_file.write(',')
    json_file.write('\n')
