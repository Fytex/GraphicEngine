import xml.dom.minidom
import xml.etree.ElementTree as ET

from typing import Tuple
from lights import Light
from camera import Camera
from element import Element
from dataclasses import dataclass
from transformations import Translation, Rotation, Scale


@dataclass
class GenElementsXML:
    file: str
    camera_options: Camera
    lights_options: Tuple[Light]
    elements: Tuple[Element]
        

    def generate(self):
        world = ET.Element("world")

        camera = ET.SubElement(world, "camera")

        lights = ET.SubElement(world, "lights")

        for light in self.lights_options:
            ET.SubElement(lights, "light", type=str(light), **light.values())
        
        group = ET.SubElement(world, "group")

        ET.SubElement(camera, "position",
                      x=self.camera_options.position_x,
                      y=self.camera_options.position_y,
                      z=self.camera_options.position_z)
        ET.SubElement(camera, "lookAt",
                      x=self.camera_options.center_x,
                      y=self.camera_options.center_y,
                      z=self.camera_options.center_z)
        ET.SubElement(camera, "up",
                      x=self.camera_options.up_x,
                      y=self.camera_options.up_y,
                      z=self.camera_options.up_z)
        ET.SubElement(camera, "projection",
                      fov=self.camera_options.fov,
                      near=self.camera_options.near,
                      far=self.camera_options.far)

        for element in self.elements:
            sub_group = ET.SubElement(group, "group")
            self._generate_group(sub_group, element)


        dom = xml.dom.minidom.parseString(ET.tostring(world))
        xml_string = dom.toprettyxml()

        with open(self.file, "w") as f:
            f.write(xml_string)

        


    def _generate_group(self, group: ET.ElementTree, element: Element):
        if element.transformations:
            transformations = ET.SubElement(group, "transform")
            

            for transformation in element.transformations:
                match transformation.__class__.__name__:
                    case "Translation":
                        if hasattr(transformation, 'time'):
                            translations = ET.SubElement(transformations, "translate", time=str(transformation.time), align=str(transformation.align))
                            for position in transformation.positions:
                                ET.SubElement(translations, "point", x=str(round(position[0], 3)), y=str(round(position[1], 3)), z=str(round(position[2], 3)))
                        else:
                            ET.SubElement(transformations, "translate", x=str(round(transformation.x, 3)), y=str(round(transformation.y, 3)), z=str(round(transformation.z, 3)))
                    case "Rotation":
                        if hasattr(transformation, 'time'):
                            ET.SubElement(transformations, "rotate", time=str(transformation.time), x=str(round(transformation.axis_x, 3)), y=str(round(transformation.axis_y, 3)), z=str(round(transformation.axis_z, 3)))
                        else:
                            ET.SubElement(transformations, "rotate", angle=str(transformation.angle), x=str(round(transformation.axis_x, 3)), y=str(round(transformation.axis_y, 3)), z=str(round(transformation.axis_z, 3)))
                    case "Scale":
                        ET.SubElement(transformations, "scale", x=str(transformation.x), y=str(transformation.y), z=str(transformation.z))


        models = ET.SubElement(group, "models")
        model = ET.SubElement(models, "model", file=element.file)

        if element.material:
            colors = ET.SubElement(model, "color")
            for component, color in element.material.components():
                ET.SubElement(colors, component, **color)
        
        if element.texture:
            ET.SubElement(model, "texture", file=element.texture)

        
        for sub_element in element.elements:
            sub_group = ET.SubElement(group, "group")
            self._generate_group(sub_group, sub_element)
                
                

        

        

        
    




