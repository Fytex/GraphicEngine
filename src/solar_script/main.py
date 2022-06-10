from random import random
from camera import Camera
from orbit import create_orbit
from gen_elements_xml import GenElementsXML
from element import Element, Material, Color, Value
from transformations import Translation, Rotation, Scale
from lights import PointLight, DirectionalLight, SpotLight

file = "solar_system.xml"


camera = Camera(
    "0", "2", "60",
    "0", "2", "0",
    "0", "1", "0",
    "60", "1", "1000"
)


lights = (
    PointLight("0", "0", "0"),
)


def get_rand():
    return round(random(), 3)

def get_3rand():
    return (get_rand() for i in range(3))

#### SUN ####
transformations = [
    Scale(6, 6, 6),
    Rotation(13, 0, get_rand(), 0)
]
sun = Element("sphere_nt.3d", transformations=transformations, texture='sun.jpg', material=Material(emissive=Color("255", "255", "255")))


#### MERCURY ####
transformations = [
    Translation(5, create_orbit((7.5, 0, 0))),
    Scale(0.5, 0.5, 0.5),
    Rotation(5, 0, get_rand(), 0)
]
mercury = Element("sphere_nt.3d", transformations=transformations, texture='mercury.jpg')


#### VENUS ####
transformations = [
    Translation(7, create_orbit((9.5, 0, 0))),
    Scale(0.9, 0.9, 0.9),
    Rotation(7, 0, get_rand(), 0)
]
venus = Element("sphere_nt.3d", transformations=transformations, texture='venus.jpg')


#### EARTH ####
moons = [
    Element("sphere_nt.3d", transformations=[Translation(5, create_orbit((0, 1.3, 0))), Scale(0.2, 0.2, 0.2), Rotation(6, *get_3rand())], texture='moon.jpg')
]

transformations = [
    Translation(10, create_orbit((12, 0, 0))),
    Scale(1, 1, 1),
    Rotation(8, 0, get_rand(), 0)
]

earth = Element("sphere_nt.3d", elements=moons, transformations=transformations, texture='earth.jpg')


#### MARS ####
moons = [
    Element("sphere_nt.3d", transformations=[Translation(20, create_orbit((0, 1.3, 0))), Scale(0.2, 0.2, 0.2), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(20, create_orbit((0, -1.7, 0))), Scale(0.1, 0.1, 0.1), Rotation(6, *get_3rand())], texture='moon.jpg')
]

transformations = [
    Translation(12, create_orbit((14, 0, 0))),
    Scale(0.7, 0.7, 0.7),
    Rotation(8, 0, get_rand(), 0)
]
mars = Element("sphere_nt.3d", elements=moons, transformations=transformations, texture='mars.jpg')


#### JUPITER ####
moons = [
    Element("sphere_nt.3d", transformations=[Translation(20, create_orbit((0, 1.3, 0))), Scale(0.15, 0.15, 0.15), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(20, create_orbit((0, -1.7, 0))), Scale(0.1, 0.1, 0.1), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(20, create_orbit((-1.4, 1.4, 1.4))), Scale(0.2, 0.2, 0.2), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(20, create_orbit((0, 1.6, 1.8))), Scale(0.12, 0.12, 0.12), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(20, create_orbit((-1, 0, 1))), Scale(0.17, 0.17, 0.17), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(20, create_orbit((1.2, 0, 1.5))), Scale(0.14, 0.14, 0.14), Rotation(6, *get_3rand())], texture='moon.jpg')
]
transformations = [
    Translation(15, create_orbit((19, 0, 0))),
    Scale(2.5, 2.5, 2.5),
    Rotation(12, 0, get_rand(), 0)
]
jupiter = Element("sphere_nt.3d", elements=moons, transformations=transformations, texture='jupiter.jpg')


#### SATURN ####
torus = Element("torus_nt.3d", texture='saturn_ring.jpg')

moons = [
    Element("sphere_nt.3d", transformations=[Translation(8, create_orbit((0, -1.3, 0))), Scale(0.15, 0.15, 0.15), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(8, create_orbit((0, 1.7, 0))), Scale(0.1, 0.1, 0.1), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(8, create_orbit((1.4, -1.4, 1.4))), Scale(0.2, 0.2, 0.2), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(8, create_orbit((0, -1.6, -1.8))), Scale(0.12, 0.12, .12), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(8, create_orbit((1, 0, -1.3))), Scale(0.17, 0.17, 0.17), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(8, create_orbit((-1.2, 0, -1.5))), Scale(0.14, 0.14, 0.14), Rotation(6, *get_3rand())], texture='moon.jpg')
]

transformations = [
    Translation(18, create_orbit((25, 0, 0))),
    Scale(2, 2, 2),
    Rotation(10, 0, get_rand(), 0)
]
saturn = Element("sphere_nt.3d", elements=[torus] + moons, transformations=transformations, texture='saturn.jpg')


#### URANUS ####
moons = [
    Element("sphere_nt.3d", transformations=[Translation(9, create_orbit((0, 1.3, -1))), Scale(0.15, 0.15, 0.15), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(9, create_orbit((1.2, -1.7, 0))), Scale(0.1, 0.1, 0.1), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(9, create_orbit((-1.2, 1.6, 1.1))), Scale(0.2, 0.2, 0.2), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(9, create_orbit((0, 1.2, 1.8))), Scale(0.12, 0.12, 0.12), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(9, create_orbit((-1, 0, 1))), Scale(0.17, 0.17, 0.17), Rotation(6, *get_3rand())], texture='moon.jpg'),
]

transformations = [
    Translation(20, create_orbit((31, 0, 0))),
    Scale(1.6, 1.6, 1.6),
    Rotation(9, 0, get_rand(), 0)
]
uranus = Element("sphere_nt.3d", elements=moons, transformations=transformations, texture='uranus.jpg')


#### NEPTUNE ####
moons = [
    Element("sphere_nt.3d", transformations=[Translation(6, create_orbit((0, 1.3, 0))), Scale(0.15, 0.15, 0.15), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(6, create_orbit((0, -1.5, 1.2))), Scale(0.1, 0.1, 0.1), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(6, create_orbit((-1.4, 1.4, 1.4))), Scale(0.2, 0.2, 0.2), Rotation(6, *get_3rand())], texture='moon.jpg'),
    Element("sphere_nt.3d", transformations=[Translation(6, create_orbit((1, 1.6, 1.8))), Scale(0.12, 0.12, 0.12), Rotation(6, *get_3rand())], texture='moon.jpg')
]

transformations = [
    Translation(25, create_orbit((35, 0, 0))),
    Scale(1.5, 1.5, 1.5),
    Rotation(9, 0, get_rand(), 0)
]
neptune = Element("sphere_nt.3d", elements=moons, transformations=transformations, texture='neptune.jpg')



#### COMET ####

transformations = [
    Translation(15, create_orbit((40, 30, 0)), True),
    Scale(0.1, 0.1, 0.1),
    Rotation(9, *get_3rand())
]

comet = Element("bezier_nt.3d", transformations=transformations, texture='comet.jpg')












solar_system = [
    sun,
    mercury,
    venus,
    earth,
    mars,
    jupiter,
    saturn,
    uranus,
    neptune,
    comet
]


if __name__ == '__main__':
    elements_xml = GenElementsXML(file, camera, lights, solar_system)
    elements_xml.generate()
