from components import Component
from abc import ABC, abstractmethod
from enums import *
import sys



    
class RigidBody(Component):

    x_lim = [sys.float_info.max, sys.float_info.min]
    y_lim = [sys.float_info.max, sys.float_info.min]
    z_lim = [sys.float_info.max, sys.float_info.min]

    tag = "none"

    def __init__(self, bodyTag : BodyTag) -> None:
        if bodyTag == BodyTag.CAR:
            self.tag = "car"
        if bodyTag == BodyTag.BALL:
            self.tag = "ball"
        if bodyTag == BodyTag.WALL:
            self.tag = "wall"
        if bodyTag == BodyTag.GOAL:
            self.tag = "goal"
        if bodyTag == BodyTag.OBSTACLE:
            self.tag = "obstacle"
        if bodyTag == BodyTag.GROUND:
            self.tag = "ground"


    def defineMeshBoundingBox(self, filepath):
        with open(filepath) as file:
            lines = file.readlines()

            for line in lines:
                if not line.startswith("v "): continue
                spl = line[2 : ].split(" ")
                
                self.x_lim = [ min(self.x_lim[0], float(spl[0])), max(self.x_lim[1], float(spl[0]))]
                self.y_lim = [ min(self.y_lim[0], float(spl[1])), max(self.y_lim[1], float(spl[1]))]
                self.z_lim = [ min(self.z_lim[0], float(spl[2])), max(self.z_lim[1], float(spl[2]))]


    def getJsonObject(self):
        return {
            "type": "Rigid Body",
            "min_x": self.x_lim[0],
            "max_x": self.x_lim[1],
            "min_y": self.y_lim[0],
            "max_y": self.y_lim[1],
            "min_z": self.z_lim[0],
            "max_z": self.z_lim[1],
            "tag" : self.tag,   
        }