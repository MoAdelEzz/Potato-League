from enum import Enum

class CamerType(Enum):
    PRESPECTIVE = "prespective"
    ORTHOGRAPHIC = "orthographic"

class CamereMode(Enum):
    FOLLOW_PLAYER = True
    FIXED = False
    
class MovementType(Enum):
    NORMAL_MODE = 0 # this is the mode where the object has the ability to transform, but needs external cause
    FIXED_VELOCITY_MODE = 1 # like a moving platform wall etc... but doesn't rotate
    FIXED_ROTATION_MODE = 2 # something that always rotates but doesn't move

class BodyType(Enum):
    CUBE = 0
    SPHERE = 1

class BodyTag(Enum):
    NONE = 0
    WALL = 1
    GROUND = 2
    BALL = 3
    CAR = 4
    GOAL = 5
    OBSTACLE = 6