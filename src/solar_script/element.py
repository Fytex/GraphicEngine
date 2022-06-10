from __future__ import annotations

from typing import Tuple
from dataclasses import dataclass, asdict
from transformations import Transformation


@dataclass
class Color:
    R: str
    G: str
    B: str

    def values(self):
        return asdict(self)

@dataclass
class Value:
    value: str

    def values(self):
        return asdict(self)


@dataclass
class Material:
    difuse: Color = None
    ambient: Color = None
    specular: Color = None
    emissive: Color = None
    shininess: Value = None

    def components(self):
        return [(k,v) for k,v in asdict(self).items() if v]
    


@dataclass
class Element:
    file: str
    elements: Tuple[Element] = tuple()
    transformations: Tuple[Transformation] = tuple()
    texture: str = None
    material: Material = None
