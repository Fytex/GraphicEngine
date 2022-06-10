from dataclasses import dataclass

@dataclass
class Camera:
    position_x: str = "0"
    position_y: str = "0"
    position_z: str = "0"
    
    center_x: str = "0"
    center_y: str = "0"
    center_z: str = "0"

    up_x: str = "0"
    up_y: str = "0"
    up_z: str = "0"

    fov: str = "0"
    near: str = "0"
    far: str = "0"
