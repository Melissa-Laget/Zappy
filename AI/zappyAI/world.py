from zappyAI.tiles import Tile
from zappyAI.struct import Size, Pos
from zappyAI.objects import Resources
from zappyAI.enums import Direction
import json


class World:
    _world: list[list[Tile]]
    _size: Size

    def __init__(self, size: Size, default_tile: Tile = Tile(False, Resources(0, 0, 0, 0, 0, 0, 0, 0))) -> None:
        self._world = [[default_tile for x in range(size.width)] for y in range(size.height)]
        self._size = size

    def __getitem__(self, pos: tuple[int, int] | Pos) -> Tile:
        if type(pos) == Pos:
            pos: tuple[int, int] = (pos.y, pos.x)
        return self._world[pos[0] % self._size.height][pos[1] % self._size.width]

    def __setitem__(self, pos: tuple[int, int] | Pos, value: Tile) -> None:
        if type(pos) == Pos:
            pos: tuple[int, int] = (pos.y, pos.x)
        self._world[pos[0] % self._size.height][pos[1] % self._size.width] = value

    def __iter__(self) -> iter:
        for y in range(self._size.height):
            for x in range(self._size.width):
                yield self._world[y][x]

    def __str__(self, player: tuple[Pos, Direction] | None = None) -> str:
        res: str = ""
        for y in range(self._size.height):
            for x in range(self._size.width):
                if player is not None and player[0].x == x and player[0].y == y:
                    if player[1] == Direction.N:
                        res += "^"
                    elif player[1] == Direction.E:
                        res += ">"
                    elif player[1] == Direction.S:
                        res += "v"
                    elif player[1] == Direction.W:
                        res += "<"
                    continue
                res += str(self._world[y][x])
            res += "\n"
        return res

    def __dict__(self) -> dict:
        res: dict = {"size": self._size.__dict__(), "tiles": []}
        for y in range(self._size.height):
            tmp = {y: []}
            for x in range(self._size.width):
                if self._world[y][x].known:
                    tmp[y].append(self._world[y][x].__dict__())
                else:
                    tmp[y].append(None)
            res["tiles"].append(tmp[y])
        return res

    def __repr__(self) -> str:
        return str({"size": self._size, "tiles": self._world})

    def __len__(self) -> int:
        return self._size.width * self._size.height

    def clear(self) -> None:
        for tile in self:
            tile._known = False

    @property
    def size(self) -> Size:
        return self._size
