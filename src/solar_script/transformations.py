from dataclasses import dataclass

class Transformation:
    pass


class Translation(Transformation):

    def __init__(self, *args):       
        if isinstance(args[1], list):
            if (len(args) == 3):
                self.time, self.positions, self.align = args
            else:
                self.time, self.positions = args
                self.align = False
            
        else:
            self.x, self.y, self.z = args


@dataclass
class Rotation(Transformation):
    time : float = 0
    axis_x: float = 0
    axis_y: float = 0
    axis_z: float = 0
    alpha : float = 0


@dataclass
class Scale(Transformation):
    x: float = 0
    y: float = 0
    z: float = 0
