from zappyAI.objects import Resources
from zappyAI.enums import ObjectType
import time


class Tile:
    _tick: int = 0
    _known: bool
    _objects: Resources

    def __init__(self, known: bool = False, objects: Resources = Resources(0, 0, 0, 0, 0, 0, 0, 0)) -> None:
        self._known = known
        self._objects = objects
        self._tick = round(time.time())

    def merge(self, other: 'Tile') -> None:
        if not other.known:
            return
        if not self.known:
            self._known = True
            self._objects = other.objects
            self._tick = other._tick
            return
        if self._tick > other._tick:
            return
        self.objects = other.objects
        self._tick = other._tick

    def __str__(self) -> str:
        if not self.known:
            return "?"
        if len(self.objects) == 0:
            return " "
        elif self.objects[ObjectType.PLAYER] > 0:
            return "P"
        else:
            return "!"

    def __repr__(self) -> str:
        return "Tile(known={}, objects={})".format(self.known, self.objects)

    def __dict__(self) -> dict:
        return {"known": self.known, "objects": self.objects.__dict__()}

    def __len__(self) -> int:
        return len(self.objects)

    def __getitem__(self, item: ObjectType | str) -> int:
        return self.objects[item]

    def __setitem__(self, item: ObjectType | str, value: int) -> None:
        self.objects[item] = value

    def __iter__(self) -> iter:
        for item in self.objects:
            for i in range(self.objects[item]):
                yield item

    @property
    def known(self) -> bool:
        return self._known

    @property
    def objects(self) -> Resources:
        return self._objects

    @objects.setter
    def objects(self, value: Resources) -> None:
        self._tick = round(time.time())
        self._objects = value
