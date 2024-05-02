from components import Component
from abc import ABC, abstractmethod
from enums import *

class FreeCameraController(Component):
    rotationSensitivity = 0.01
    fovSensitivity = 0.3
    positionSensitivity = [3.0, 3.0, 3.0]
    speedupFactor = 5.0

    def getJsonObject(self):
        return {
            "type": "Free Camera Controller",
            "rotationSensitivity": self.rotationSensitivity,
            "fovSensitivity": self.fovSensitivity,
            "positionSensitivity": self.positionSensitivity,
            "speedupFactor": self.speedupFactor
        }
    

class PlayerController(Component):
    rotationSensitivity = 0.5
    speedupFactor = 0.2

    def getJsonObject(self):
        return {"type": "Player","rotationSensitivity": self.rotationSensitivity, "speedupFactor": self.speedupFactor}
    
def createController(followMode):
    if followMode:
        return PlayerController()
    else:
        return FreeCameraController()

class CameraComponent(Component):
    cameraType : CamerType = CamerType.PRESPECTIVE
    cameraMode : CamereMode = CamereMode.FOLLOW_PLAYER

    def __init__(self) -> None:
        super().__init__()
        self.type = "Camera"

    # prespective camera parameters
    near = 0.01
    far = 100.0
    fovY = 90

    def configurePrespectiveCamera(self, nearZ = 0.01, farZ = 100.0, fovyAngle = 90):
        """
        configure the camera type to prespective camera where the inputs as follows
        nearZ => near plane z value, default = 0.01
        farZ => far plane z value, default = 100
        fovyAngle => field of view angle, default = 90
        """
        self.cameraType = CamerType.PRESPECTIVE
        self.near = nearZ
        self.far = farZ
        self.fovY = fovyAngle 

    # orthographic camera parameters
    orthoHeight = 1.0
    
    def configreOrthographicCamera(self, cubeHeight = 1.0):
        """
        the only parameter is the height of the cube in the homogenous clip space, default = 1.0
        """
        self.cameraType = CamerType.ORTHOGRAPHIC
        self.orthoHeight = cubeHeight


    def getCameraTypeConfig(self):
        if self.cameraType == CamerType.PRESPECTIVE:
            return {
                "cameraType": "prespective",
                "near": self.near,
                "far": self.far,
                "fovY": self.fovY
            }
        else:
            return {
                "cameraType": "orthographic",
                "orthoHeight": self.orthoHeight
            }

    # follow mode parameters
    distance = 2.0
    height = 2.0
    followOwner = False

    def configureFixedMode(self):
        """
        normal configuration as phase 1
        """
        followOwner = False

    def configureFollowMode(self, distance_from_the_object, height_above_the_object):
        """
        this function configure the camera to operate in the follow mode where it will always follow
        its owner position in world minus the first parameter (distance_from_the_object) and higher than object level
        with the second argument (height_above_the_object)

        all values are in world space
        """
        self.followOwner = True
        self.distance = distance_from_the_object
        self.height = height_above_the_object

    def getCameraModeConfig(self):
        if self.followOwner:
            return {"followOwner": True, "distance": self.distance, "height": self.height}
        else:
            return {"followOwner": False}

    def getJsonObject(self):
        return {"type": self.type, **self.getCameraTypeConfig(), **self.getCameraModeConfig()}
