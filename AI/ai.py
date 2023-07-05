import base64
import os
import sys
import json
import zappyAI as zp
import utils

OBJECTIVES = [
    zp.Resources(0, 1, 0, 0, 0, 0, 0, 0),
    zp.Resources(0, 1, 1, 1, 0, 0, 0, 1),
    zp.Resources(0, 2, 0, 1, 0, 2, 0, 1),
    zp.Resources(0, 1, 1, 2, 0, 1, 0, 3),
    zp.Resources(0, 1, 2, 1, 3, 0, 0, 3),
    zp.Resources(0, 1, 2, 3, 0, 1, 0, 5),
    zp.Resources(0, 2, 2, 2, 2, 2, 1, 5),
    zp.Resources(0, 0, 0, 0, 0, 0, 0, 0)  # Last one must be empty
]

FOOD_WANTED = 10

INCANTATION_RATE = 150


class AI:
    _encrypt: bool
    _auto_spawn: bool
    msg_handler: 'Message'
    _msg_key: bytes
    _direction: zp.Direction = zp.Direction.N
    _comm: utils.Comm
    _pos: zp.Pos = zp.Pos(5, 5)
    _teamName: str
    _level: int = 1
    _world: zp.World
    _ticks: int = 0
    _inventory: zp.Resources = zp.Resources(0, 0, 0, 0, 0, 0, 0, 0)
    role: zp.Role | None = None
    id: int = os.getpid()
    master_id: int | None = None
    master_found: bool = False
    _messages: list[tuple[zp.Direction, dict]] = []
    _buffer: list[str] = []
    elevation: bool = False

    def _new(self) -> None:
        if not self._auto_spawn:
            print("====================You must spawn manually====================")
            return
        pid = os.fork()
        if pid == 0:
            python = sys.executable
            os.execl(python, python, *sys.argv)
            exit(0)
        if pid < 0:
            raise RuntimeError("Failed to fork")

    def __init__(self, comm: utils.Comm, team_name: str, encrypt: bool, auto_spawn: bool) -> None:
        self._comm = comm
        self._teamName = team_name
        self._encrypt = encrypt
        self._auto_spawn = auto_spawn

    def connection_error(self, data) -> None:
        print("\033[1;31m==> ERROR:", data)
        self._buffer = []
        self._messages = []
        self.recv(False)

    def add_message(self, msg: 'Message') -> None:
        self.msg_handler = msg

    def start(self) -> None:
        data: list[str] = self._comm.recv()
        if data != [utils.WELCOME]:
            return self.connection_error(data)
        self._login(self._teamName)
        self.check_inventory()
        self._msg_key = utils.generate_key(self._teamName)
        if self.connect_nbr() > 0:
            self.role = zp.Role.WORKER
            print(self.id, ": I'm a worker")
            self.check_inventory()
            self._new()
            return
        print(self.id, " : I'm the master")
        self.broadcast(self.msg_handler["bootstrap master"].to_json())
        while not self.run_message("bootstrap master", None):
            self.recv(False)
        print("Master id: ", self.master_id)

    def _add_time(self, time: int) -> None:
        for i in range(time):
            self._ticks += 1
            if self._ticks % 126 == 0:
                self._inventory[zp.ObjectType.FOOD] -= 1
                if self._inventory[zp.ObjectType.FOOD] <= 0:
                    self.check_inventory()

    def _check_time(self, time: int) -> int:
        current: int = self._ticks
        food_left: int = self._inventory[zp.ObjectType.FOOD]
        for i in range(time):
            current += 1
            if current % 126 == 0:
                food_left -= 1
        print("Food left: ", food_left)
        return food_left

    def on_eject(self, msg: str):
        print("Ejected")
        old_direction: zp.Direction = self._direction
        goto_direction: zp.Direction = zp.Direction(int(msg[len(utils.EJECT_MSG):]))
        while goto_direction.value > 8:
            goto_direction.value -= 8
        while self._direction != goto_direction:
            self.left()
        self.forward()
        while self._direction != old_direction:
            self.right()

    def draw_map(self) -> None:
        print(self._world.__str__((self._pos, self._direction)))
        print(str(self._inventory))
        print("pos:", self.pos)
        print("lvl:", self._level)

    def parse_msg(self, line: str) -> tuple[zp.Direction, str] | None:
        try:
            direction: zp.Direction = zp.Direction(
                int(line[len(utils.BROADCAST_MSG_START):(len(utils.BROADCAST_MSG_START) + 1)]))
        except ValueError:
            return self.connection_error(line)
        return direction, line[(len(utils.BROADCAST_MSG_START) + 2):]

    def _exec_msg(self) -> None:
        len_msgs: int = len(self._messages)
        while 0 < len_msgs:
            try:
                current = self._messages.pop(0)
            except IndexError:
                return
            try:
                self.msg_handler[current[1]["what"]](current[0], current[1])
            except KeyError:
                pass
            len_msgs -= 1

    def _exec_elevation(self) -> None:
        len_buf: int = len(self._buffer)
        while 0 < len_buf:
            try:
                current = self._buffer.pop(0)
            except IndexError:
                return
            if current == utils.ELEVATION_UNDERWAY:
                self.elevate()
            elif current.startswith(utils.ELEVATION_SUCCESS):
                self.new_level(current)
            len_buf -= 1

    def exec_buffer(self) -> None:
        self._exec_elevation()
        self._exec_msg()

    def run_message(self, what: str, sender: int | None) -> bool:
        i: int = 0
        len_msgs: int = len(self._messages)
        while i < len_msgs:
            if self._messages[i][1]["what"] == what and (sender is None or self._messages[i][1]["from"] == sender):
                current = self._messages.pop(i)
                try:
                    self.msg_handler[what](current[0], current[1])
                except KeyError:
                    pass
                return True
            i += 1
        return False

    def decode_msg(self, msg: str) -> None:
        data = self.parse_msg(msg)
        if data is None:
            return
        direction, message = data
        if self._encrypt:
            message: str = base64.b64decode(utils.decrypt(message.encode(), self._msg_key)).decode("utf-8")
            print("Message from " + str(direction) + ": " + message)
        else:
            print("Message from " + str(direction))
        try:
            res: dict = json.loads(message)
        except json.JSONDecodeError:
            return
        if "from" not in res or "to" not in res or "what" not in res or "ans" not in res:
            return
        if res["to"] is not None and res["to"] != self.id:
            return
        if res["from"] == self.id:
            return
        self._messages.append((direction, res))

    def elevate(self) -> None:
        self._add_time(300)
        print(utils.MAGENTA)
        # res = self.recv(True, True)
        # for msg in res:
        #     if msg.startswith(utils.ELEVATION_SUCCESS):
        #         self.new_level(msg)
        #         return

    def new_level(self, msg: str) -> None:
        print(utils.BLUE)
        print("==============================NEW LEVEL==============================")
        try:
            self._level = int(msg[len(utils.ELEVATION_SUCCESS):])
            print("New level: ", self._level)
            # os.system("notify-send 'New level (" + str(self.id) + "): " + str(self._level) + "'")
        except ValueError:
            return self.connection_error(msg)
        self.elevation = False
        self.check_inventory()
        rec: int = 8
        while self.inventory.food < 7 and self.take(zp.ObjectType.FOOD) and rec > 0:
            rec -= 1

    def recv(self, exec_buffer: bool = True, incantation: bool = False) -> list[str]:
        i: int = 0
        data: list[str] = self._comm.recv()
        len_data: int = len(data)
        force_buffer: bool = False
        while i < len_data:
            print("len: ", len(data[i]))
            if data[i] == utils.DEAD:
                data.pop(i)
                len_data -= 1
                raise TimeoutError("Dead")
            elif data[i] == utils.EJECT_MSG:
                self.on_eject(data.pop(i))
                len_data -= 1
            elif (not incantation) and data[i] == utils.ELEVATION_UNDERWAY:
                self._buffer.append(data.pop(i))
                len_data -= 1
                pass
            elif (not incantation) and data[i].startswith(utils.ELEVATION_SUCCESS):
                force_buffer = True
                self._buffer.append(data.pop(i))
                len_data -= 1
                pass
            elif data[i].startswith(utils.BROADCAST_MSG_START):
                msg = data.pop(i)
                len_data -= 1
                self.decode_msg(msg)
            else:
                i += 1
        if force_buffer:
            self._exec_elevation()
            return ["ko"]
        if len(data) == 0 and exec_buffer:
            return self.recv()
        if exec_buffer:
            self.exec_buffer()
        return data

    def _login(self, team_name: str) -> None:
        res: tuple[int, tuple[int, int]]
        data: list[str]

        self._comm.send(team_name + "\n")
        data = self.recv()
        if len(data) != 2:
            return self.connection_error(data)
        try:
            res = (int(data[0]),
                   (int(data[1].split(" ")[0]), int(data[1].split(" ")[1])))
        except ValueError:
            return self.connection_error(data)
        self._world = zp.World(zp.Size(res[1][1], res[1][0]))

    def _get_objects(self, data: str) -> list[zp.Resources]:
        objects: list[zp.Resources] = []
        if not data.startswith("[") or not data.endswith("]"):
            self.connection_error(data)
            return []
        tmp: list[str | None] = data[2:-2].split(",")
        for i in range(len(tmp)):
            if tmp[i] == "None":
                tmp[i] = None
                continue
            tmp[i] = tmp[i].strip()
        for index, objs in enumerate(tmp):
            if objs is None:
                objects.append(zp.Resources(0, 0, 0, 0, 0, 0, 0, 0))
                continue
            current = zp.Resources(0, 0, 0, 0, 0, 0, 0, 0)
            for idx, obj in enumerate(objs.split(" ")):
                if obj.strip() == "" or (index == 0 and idx == 0 and obj.strip() == "player"):
                    continue
                current[obj] += 1
            objects.append(current)
        return objects

    def forward(self) -> None:
        self._comm.send("Forward\n")
        if self._direction == zp.Direction.N:
            self._pos.y = (self._pos.y - 1) % self._world.size.height
        elif self._direction == zp.Direction.E:
            self._pos.x = (self._pos.x + 1) % self._world.size.width
        elif self._direction == zp.Direction.S:
            self._pos.y = (self._pos.y + 1) % self._world.size.height
        elif self._direction == zp.Direction.W:
            self._pos.x = (self._pos.x - 1) % self._world.size.width
        else:
            raise ValueError("Invalid direction")
        data: list[str] = self.recv()
        if data != [utils.OK]:
            return self.connection_error(data)
        self._add_time(7)

    def right(self) -> None:
        self._comm.send("Right\n")
        self._add_time(7)
        self._direction = zp.Direction(
            (self._direction.value - (1 if self._direction.value % 2 == 0 else 2) if self._direction.value > 2 else 7))
        data: list[str] = self.recv()
        if data != [utils.OK]:
            return self.connection_error(data)

    def left(self) -> None:
        self._comm.send("Left\n")
        self._add_time(7)
        self._direction = zp.Direction(
            (self._direction.value + (1 if self._direction.value % 2 == 0 else 2) if self._direction.value < 6 else 1))
        data: list[str] = self.recv()
        if data != [utils.OK]:
            return self.connection_error(data)

    def _at(self, relative_pos: tuple[int, int] | zp.Pos) -> tuple[int, int]:
        if type(relative_pos) == zp.Pos:
            relative_pos = (relative_pos.y, relative_pos.x)
        y, x = relative_pos
        if self._direction == zp.Direction.S:
            return self._pos.y + y, self._pos.x + x
        elif self._direction == zp.Direction.E:
            return self._pos.y + x, self._pos.x + y
        elif self._direction == zp.Direction.N:
            return self._pos.y - y, self._pos.x + x
        elif self._direction == zp.Direction.W:
            return self._pos.y + x, self._pos.x - y
        else:
            raise ValueError("Invalid direction")

    def look(self) -> list[tuple[zp.Pos, zp.Resources]]:
        nb_tiles: list[int] = [1, 3, 5, 7, 9, 11, 13, 15, 17]
        self._comm.send("Look\n")
        self._add_time(7)
        data: list[str] = self.recv()
        res: list[zp.Resources] = self._get_objects(data[0])
        dump: list[tuple[zp.Pos, zp.Resources]] = []
        # if len(res) - 1 != nb_tiles[self._level]:
        #     print("\033[1;31m==> ERROR:", data)
        #     raise ConnectionError("Invalid response:")
        for lv in range(self._level + 1):
            for i in range(-lv, nb_tiles[lv] - lv):
                pos = self._at((lv, i))
                try:
                    self._world[(pos[0], pos[1])] = zp.Tile(True, res.pop(0))
                    dump.append((zp.Pos(pos[0], pos[1]), self._world[(pos[0], pos[1])].objects))
                except IndexError:
                    pass
                except AttributeError:
                    pass
        return dump

    def check_inventory(self) -> bool:
        self._comm.send("Inventory\n")
        self._add_time(1)
        success: bool = True
        data: list[str] = self.recv()
        datalist: list[str]
        if len(data) != 1:
            self.connection_error(data)
            return False
        if not data[0].startswith("[ ") or not data[0].endswith(" ]"):
            self.connection_error(data)
            return False
        data[0] = data[0][2:-2]
        datalist = data[0].split(", ")
        if len(datalist) != 7:
            self.connection_error(data)
            return False
        for string in datalist:
            try:
                key, value = string.strip().split(" ")
                if key not in self._inventory or not value.isdigit():
                    raise ValueError
                if int(value) != self._inventory[key]:
                    success = False
                self._inventory[key] = int(value)
            except:
                self.connection_error(data)
                return False
        return success

    def broadcast(self, message: str | None) -> None:
        if message is None:
            return
        if self._encrypt:
            print("raw message: " + message)
            encoded: bytes = base64.b64encode(message.encode())
            message: str = utils.encrypt(encoded, self._msg_key).decode("utf-8")
        self._comm.send("Broadcast " + message + "\n")
        self._add_time(7)
        data: list[str] = self.recv()
        if data != [utils.OK]:
            return self.connection_error(data)

    def connect_nbr(self) -> int:
        nb_connect: int = 0
        data: list[str]
        self._comm.send("Connect_nbr\n")
        data = self.recv()
        if len(data) != 1:
            self.connection_error(data)
            return 0
        nb_connect = int(data[0])
        return nb_connect

    def fork(self) -> None:
        self._comm.send("Fork\n")
        self._add_time(42)
        data: list[str] = self.recv()
        if data != [utils.OK]:
            return self.connection_error(data)
        # os.system("notify-send 'new one created'")
        self._new()

    def eject(self) -> None:
        self._comm.send("Eject\n")
        self._add_time(7)
        data: list[str] = self.recv()
        if data != [utils.OK]:
            return self.connection_error(data)

    def take(self, resource: zp.ObjectType) -> bool:
        if resource not in self._inventory or resource == zp.ObjectType.PLAYER:
            return False
        if resource != zp.ObjectType.FOOD and not self.elevation and self.pos in [zp.Pos(5, 5), zp.Pos(5, 6),
                                                                                  zp.Pos(6, 5), zp.Pos(5, 4),
                                                                                  zp.Pos(4, 5)]:
            return False
        self._comm.send("Take " + str(resource) + "\n")
        self._add_time(7)
        res: list[str] = self.recv()
        if res == [utils.OK]:
            self._inventory[resource] += 1
            self._world[(self._pos.y, self._pos.x)].objects[resource] -= 1
            if self._world[(self._pos.y, self._pos.x)].objects[resource] <= 0:
                self.look()
            return True
        elif res == [utils.KO]:
            return False
        self.connection_error(res)
        return False

    def set(self, resource: zp.ObjectType) -> bool:
        if resource not in self._inventory or self._inventory[resource] <= 0:
            return False
        self._comm.send("Set " + str(resource) + "\n")
        self._add_time(7)
        res: list[str] = self.recv()
        if res == [utils.OK]:
            self._inventory[resource] -= 1
            self._world[(self._pos.y, self._pos.x)].objects[resource] += 1
            return True
        elif res == [utils.KO]:
            return False
        self.connection_error(res)
        return False

    def incantation(self, drop_items: bool = True) -> bool:
        self.look()
        if drop_items:
            self.check_inventory()
            for resource in self._inventory:
                if resource == zp.ObjectType.PLAYER:
                    continue
                missing: int = OBJECTIVES[self._level - 1][resource] - self._world[(self._pos.y, self._pos.x)].objects[
                    resource]
                if self._inventory[resource] < missing:
                    raise ValueError("Not enough resources (drop)")
            # this code is duplicated because we need check all resources before dropping them
            for resource in self._inventory:
                if resource == zp.ObjectType.PLAYER:
                    continue
                missing: int = OBJECTIVES[self._level - 1][resource] - self._world[(self._pos.y, self._pos.x)].objects[
                    resource]
                for _ in range(missing):
                    self.set(resource)
        for resource in self._world[(self._pos.y, self._pos.x)].objects:
            if self._world[(self._pos.y, self._pos.x)].objects[resource] < OBJECTIVES[self._level - 1][resource]:
                raise ValueError("Not enough resources")
        # if self._check_time(300) < 1:
        #     raise TimeoutError("Not enough time")
        self._comm.send("Incantation\n")
        # res = self.recv(True, True)
        # if res == [utils.KO]:
        #     return False
        # for ans in res:
        #     if ans == utils.ELEVATION_UNDERWAY:
        #         self.elevate()
        #         return True
        # self.connection_error(res)
        return True

    def __dict__(self) -> dict:
        return {
            "pos": self._pos.__dict__(),
            "world": self._world.__dict__(),
            "inventory": self._inventory.__dict__(),
            "level": self._level,
            "ticks": self._ticks,
            "direction": self._direction.__repr__()
        }

    def to_json(self) -> str:
        return json.dumps(self.__dict__())

    @property
    def pos(self) -> zp.Pos:
        return self._pos

    @property
    def world(self) -> zp.World:
        return self._world

    @property
    def inventory(self) -> zp.Resources:
        return self._inventory

    @property
    def level(self) -> int:
        return self._level

    @property
    def ticks(self) -> int:
        return self._ticks

    @property
    def direction(self) -> zp.Direction:
        return self._direction
