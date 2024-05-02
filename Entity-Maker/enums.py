from enum import Enum

class CamerType(Enum):
    PRESPECTIVE = "prespective"
    ORTHOGRAPHIC = "orthographic"

class CamereMode(Enum):
    FOLLOW_PLAYER = True
    FIXED = False
    
class MovementType(Enum):
    NORMAL_MODE = 0, # this is the mode where the object has the ability to transform, but needs external cause
    FIXED_VELOCITY_MODE = 1, # like a moving platform wall etc... but doesn't rotate
    FIXED_ROTATION_MODE = 2 # something that always rotates but doesn't move