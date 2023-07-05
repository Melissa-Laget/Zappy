from dataclasses import dataclass


@dataclass
class Pos:
    y: int
    x: int

    def __dict__(self) -> dict:
        return {"x": self.x, "y": self.y}


@dataclass
class Size:
    height: int
    width: int

    def __dict__(self) -> dict:
        return {"width": self.width, "height": self.height}
