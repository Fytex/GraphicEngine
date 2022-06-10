from dataclasses import dataclass, asdict

@dataclass
class Light:
    def values(self):
        return asdict(self)


@dataclass
class PointLight(Light):
    posX: str
    posY: str
    posZ: str

    def __str__(self):
        return 'point'


@dataclass
class DirectionalLight(Light):
    dirX: str
    dirY: str
    dirZ: str

    def __str__(self):
        return 'directional'

@dataclass
class SpotLight(Light):
    posX: str
    posY: str
    posZ: str
    dirX: str
    dirY: str
    dirZ: str
    cutoff: str

    def __str__(self):
        return 'spotlight'
