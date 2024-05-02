from components import Component
from abc import ABC, abstractmethod
from enums import MovementType

class Movement(Component):
    movementType: MovementType

    forward = [0, 0, -1]
    canRoll = False

    inital_linear_speed = 0
    initial_angular_velocity = [0, 0, 0]

    max_positive_speed = 32.0
    max_negative_speed = 8.0

    max_angular_velocity = 6.0

    linear_slowdown_factor = 2.0
    angular_slowdown_factor = 0.002

    def __init__(self, type: MovementType) -> None:
        self.movementType = type

    def setSpeedLimits(self, Vneg = 8, Vpos = 32):
        self.max_negative_speed = Vneg
        self.max_positive_speed = Vpos

    def setMaxRotationsSpeed(self, maxSpeed):
        self.max_angular_velocity = maxSpeed

    def setInitialConditions(self, initial_speed = 0, initial_rotation_velocity = [0, 0, 0], forward_vector = [0, 0, -1]):
        self.inital_linear_speed = initial_speed
        self.initial_angular_velocity = initial_rotation_velocity
        self.forward = forward_vector

    def addRollAnimation(self):
        self.canRoll = True

    def resolveMovementType(self, movementType):
        if movementType == MovementType.NORMAL_MODE : return "normal"
        if movementType == MovementType.FIXED_VELOCITY_MODE : return "fixed_direction"
        if movementType == MovementType.FIXED_ROTATION_MODE : return "fixed_rotation"


    def getJsonObject(self):

        return {
            "type": "Movement",
            "movementType": self.resolveMovementType(self.movementType),

            "forward": self.forward,
            "canRoll": self.canRoll,

            "initial_speed": self.inital_linear_speed,
            "initial_angular_velocity": self.initial_angular_velocity,

            "max_positive_speed": self.max_positive_speed,
            "max_negative_speed": self.max_negative_speed,

            "max_angular_velocity": self.max_angular_velocity,

            "linear_slowdown_factor": self.linear_slowdown_factor,
            "angular_slowdown_factor": self.angular_slowdown_factor,
        }