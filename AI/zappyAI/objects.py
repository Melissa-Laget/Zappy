from dataclasses import dataclass
from zappyAI.enums import *


@dataclass
class Resources:
    food: int
    linemate: int
    deraumere: int
    sibur: int
    mendiane: int
    phiras: int
    thystame: int
    player: int

    def __getitem__(self, item: ObjectType | str) -> int:
        if type(item) == str:
            if item == "food":
                return self.food
            elif item == "linemate":
                return self.linemate
            elif item == "deraumere":
                return self.deraumere
            elif item == "sibur":
                return self.sibur
            elif item == "mendiane":
                return self.mendiane
            elif item == "phiras":
                return self.phiras
            elif item == "thystame":
                return self.thystame
            elif item == "player":
                return self.player
            else:
                raise ValueError("Invalid name")
        elif type(item) == ObjectType:
            if item == ObjectType.FOOD:
                return self.food
            elif item == ObjectType.LINEMATE:
                return self.linemate
            elif item == ObjectType.DERAUMERE:
                return self.deraumere
            elif item == ObjectType.SIBUR:
                return self.sibur
            elif item == ObjectType.MENDIANE:
                return self.mendiane
            elif item == ObjectType.PHIRAS:
                return self.phiras
            elif item == ObjectType.THYSTAME:
                return self.thystame
            elif item == ObjectType.PLAYER:
                return self.player
            else:
                raise ValueError("Invalid ObjectType")
        else:
            raise ValueError("Invalid object type")

    def __setitem__(self, item: ObjectType | str, value: int) -> None:
        if type(item) == str:
            if item == "food":
                self.food = value
            elif item == "linemate":
                self.linemate = value
            elif item == "deraumere":
                self.deraumere = value
            elif item == "sibur":
                self.sibur = value
            elif item == "mendiane":
                self.mendiane = value
            elif item == "phiras":
                self.phiras = value
            elif item == "thystame":
                self.thystame = value
            elif item == "player":
                self.player = value
            else:
                raise ValueError("Invalid name")
        elif type(item) == ObjectType:
            if item == ObjectType.FOOD:
                self.food = value
            elif item == ObjectType.LINEMATE:
                self.linemate = value
            elif item == ObjectType.DERAUMERE:
                self.deraumere = value
            elif item == ObjectType.SIBUR:
                self.sibur = value
            elif item == ObjectType.MENDIANE:
                self.mendiane = value
            elif item == ObjectType.PHIRAS:
                self.phiras = value
            elif item == ObjectType.THYSTAME:
                self.thystame = value
            elif item == ObjectType.PLAYER:
                self.player = value
            else:
                raise ValueError("Invalid ObjectType")
        else:
            raise ValueError("Invalid object type")

    def __contains__(self, item: ObjectType | str) -> bool:
        try:
            self[item] = self[item]
        except ValueError:
            return False
        return True

    def __len__(self) -> int:
        res: int = 0
        for key in self:
            res += self[key]
        return res

    def __str__(self) -> str:
        return f"Resources(food={self.food}, linemate={self.linemate}, deraumere={self.deraumere}, sibur={self.sibur}, mendiane={self.mendiane}, phiras={self.phiras}, thystame={self.thystame}, player={self.player})"

    def __repr__(self) -> str:
        return str(self)

    def __dict__(self) -> dict:
        return {
            "food": self.food,
            "linemate": self.linemate,
            "deraumere": self.deraumere,
            "sibur": self.sibur,
            "mendiane": self.mendiane,
            "phiras": self.phiras,
            "thystame": self.thystame,
            "player": self.player
        }

    def __iter__(self) -> iter:
        yield ObjectType.FOOD
        yield ObjectType.LINEMATE
        yield ObjectType.DERAUMERE
        yield ObjectType.SIBUR
        yield ObjectType.MENDIANE
        yield ObjectType.PHIRAS
        yield ObjectType.THYSTAME
        yield ObjectType.PLAYER

    def __add__(self, other: 'Resources') -> 'Resources':
        return Resources(
            self.food + other.food,
            self.linemate + other.linemate,
            self.deraumere + other.deraumere,
            self.sibur + other.sibur,
            self.mendiane + other.mendiane,
            self.phiras + other.phiras,
            self.thystame + other.thystame,
            self.player + other.player
        )

    def __sub__(self, other: 'Resources') -> 'Resources':
        return Resources(
            self.food - other.food,
            self.linemate - other.linemate,
            self.deraumere - other.deraumere,
            self.sibur - other.sibur,
            self.mendiane - other.mendiane,
            self.phiras - other.phiras,
            self.thystame - other.thystame,
            self.player - other.player
        )

    def __eq__(self, other: 'Resources') -> bool:
        return (self.food == other.food and
                self.linemate == other.linemate and
                self.deraumere == other.deraumere and
                self.sibur == other.sibur and
                self.mendiane == other.mendiane and
                self.phiras == other.phiras and
                self.thystame == other.thystame and
                self.player == other.player)

    def __ne__(self, other: 'Resources') -> bool:
        return not self.__eq__(other)

    def is_empty(self) -> bool:
        for i in self:
            if self[i] != 0:
                return False
        return True


def is_object(name: str) -> bool:
    if name in ObjectType.__iter__():
        return True
    return False
