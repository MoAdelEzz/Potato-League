from abc import ABC, abstractmethod

class Component(ABC):
    type = ""

    @abstractmethod
    def getJsonObject(self):
        pass



# class Light(Component):
#     def getJsonObject(self):
#         return super().getJsonObject()

# class MeshRenderer(Component):
#     def getJsonObject(self):
#         return super().getJsonObject()


    
# class RigidBody(Component):
#     def getJsonObject(self):
#         return super().getJsonObject() 
    


