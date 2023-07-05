import logging
import random
import ai
import zappyAI as zp
import ai as zp_ai
import utils
from typing import Callable
from dataclasses import dataclass
import json
import time


class Brain:
    ai: zp_ai.AI
    _look_for: zp.Resources = zp.Resources(0, 0, 0, 0, 0, 0, 0, 0)
    _status: list[zp.Status] = [zp.Status.NOTHING]
    _cartography_last_pos: zp.Pos = zp.Pos(0, 0)
    _vision: list[tuple[zp.Pos, zp.Resources]] = []
    food_wanted: int = zp_ai.FOOD_WANTED
    master_direction: zp.Direction = zp.Direction.N
    players: list[int] = []
    _all_spawned: bool = False
    _fork: bool

    def __init__(self, team_name: str, comm: utils.Comm, encrypt: bool, auto_spawn: bool, fork: bool) -> None:
        self._fork = fork
        self.ai = zp_ai.AI(comm, team_name, encrypt, auto_spawn)
        self.ai.add_message(Message(self))
        self.ai.start()
        self.change_status(zp.Status.SEARCHING)

    def log_status(self) -> None:
        if self._status[-1] == zp.Status.DYING or self._status[-1] == zp.Status.HEALING:
            print(utils.RED, end="")
        elif self._status[-1] == zp.Status.SEARCHING:
            print(utils.GREEN, end="")
        elif self._status[-1] == zp.Status.MOVING:
            print(utils.YELLOW, end="")
        elif self._status[-1] == zp.Status.NOTHING:
            print(utils.WHITE, end="")

    def change_status(self, status: zp.Status | None = None) -> None:
        if status is None:
            if len(self._status) > 1:
                self._status.pop()
        else:
            self._status.append(status)
        self.log_status()

    def _action(self, evt: zp.Evt) -> None:
        self.log_status()
        if self.ai.world[self.ai.pos].known:
            if self.ai.id in [1, 2, 3, 4] and self.ai.pos == zp.Pos(5, 5):
                pass
            else:
                while self.ai.world[self.ai.pos].objects[zp.ObjectType.FOOD] > 0 and \
                        self.ai.inventory.food < self.food_wanted + 5:
                    self.take(zp.ObjectType.FOOD)
        if self._status[-1] == zp.Status.HEALING:
            self.healing(evt)
        if self.ai.inventory[zp.ObjectType.FOOD] < 3:
            self.dying(evt)
        if self._status[-1] == zp.Status.SEARCHING:
            self.searching(evt)
        elif self._status[-1] == zp.Status.MOVING:
            self.moving(evt)

    def on_tile(self) -> None:
        self._vision = []
        self._action(zp.Evt.ON_TILE)
        self.ai.draw_map()

    def on_look(self) -> None:
        self._vision = self.ai.look()
        self._action(zp.Evt.ON_LOOK)
        self.ai.draw_map()

    def on_turn(self) -> None:
        self._vision = []
        self.ai.draw_map()

    def dying(self, evt: zp.Evt, go_back: bool = True) -> None:
        if self.ai.elevation:
            if self._status[-1] == zp.Status.DYING:
                self.change_status()
            self.take(zp.ObjectType.FOOD)
            return
        if self._status[-1] == zp.Status.DYING and not self.ai.inventory.food < self.food_wanted // 2:
            self.change_status()
            return
        self.change_status(zp.Status.HEALING)
        prev_pos = self.ai.pos
        prev_dir = self.ai.direction
        while self.ai.inventory.food < self.food_wanted // 2:
            # if not self.go_where(zp.ObjectType.FOOD, True, 0):
            self.ai.look()
            for (pos, resources) in self._vision:
                for resource in resources:
                    if resource != zp.ObjectType.FOOD:
                        continue
                    self._go_take(pos, zp.Resources(self.food_wanted // 2, 0, 0, 0, 0, 0, 0, 0), True)
            if random.randint(0, 4) == 0:
                self.ai.right()
            self.forward()
        if go_back:
            self.goto(prev_pos)
            self.turn(prev_dir)
        self.change_status()

    def healing(self, evt: zp.Evt) -> None:
        print("inventory:", self.ai.inventory)

    def take_max(self, resource: zp.ObjectType, pos: zp.Pos, max_item: int) -> None:
        if self.ai.world[pos].objects[resource] > 0 and self.ai.inventory[resource] < max_item:
            self.goto(pos)
            while self.ai.world[pos].objects[resource] > 0 and self.ai.inventory[resource] < max_item:
                if not self.take(resource):
                    break

    def _go_take(self, pos: zp.Pos, resources: zp.Resources, go_back: bool = True) -> None:
        self.change_status(zp.Status.MOVING)
        current: zp.Pos = self.ai.pos
        self.take_max(zp.ObjectType.FOOD, pos, self.food_wanted)
        for resource in resources:
            if resource == zp.ObjectType.FOOD:
                continue
            self.take_max(resource, pos, resources[resource])
        if go_back:
            self.goto(current)
        self.change_status()

    def _searching(self, evt: zp.Evt, lvl: int) -> None:
        for resource in zp_ai.OBJECTIVES[lvl - 1]:
            if resource == zp.ObjectType.PLAYER:
                continue
            while self.ai.inventory[resource] < zp_ai.OBJECTIVES[lvl - 1][resource] and \
                    self.ai.world[self.ai.pos].objects[resource] > 0:
                if not self.take(resource):
                    break
        if evt == zp.Evt.ON_LOOK:
            for (pos, resources) in self._vision:
                for resource in resources:
                    if resource == zp.ObjectType.PLAYER:
                        continue
                    self._go_take(pos, zp_ai.OBJECTIVES[lvl - 1], True)

    def searching(self, evt: zp.Evt) -> None:
        if not self.ai.world[self.ai.pos].known:
            return
        self._searching(evt, self.ai.level)
        # self._searching(evt, self.ai.level + 1)

    def moving(self, evt: zp.Evt) -> None:
        pass

    def take(self, resource: zp.ObjectType) -> bool:
        if not self.ai.take(resource):
            self.ai.look()
            if self.ai.world[self.ai.pos].objects[resource] > 0:
                return self.ai.take(resource)
            else:
                return False
        return True

    def forward(self) -> None:
        self.ai.forward()
        self.on_tile()

    def turn(self, direction: zp.Direction) -> None:
        if direction.value % 2 == 0:
            if direction.value == 8:
                direction.value = 1
            else:
                direction.value += 1
        while self.ai.direction != direction:
            self.on_turn()
            if self.ai.direction < direction:
                self.ai.left()
            else:
                self.ai.right()

    def up(self) -> None:
        self.turn(zp.Direction.N)
        self.forward()

    def down(self) -> None:
        self.turn(zp.Direction.S)
        self.forward()

    def left(self) -> None:
        self.turn(zp.Direction.W)
        self.forward()

    def right(self) -> None:
        self.turn(zp.Direction.E)
        self.forward()

    def goto(self, pos: zp.Pos, look: bool = False) -> None:
        while pos.y >= self.ai.world.size.height:
            pos.y -= self.ai.world.size.height
        if pos.y < 0:
            pos.y = 0
        while pos.x >= self.ai.world.size.width:
            pos.x -= self.ai.world.size.width
        if pos.x < 0:
            pos.x = 0
        while self.ai.pos != pos:
            print("current:", self.ai.pos)
            print("target:", pos)
            if self.ai.pos.x < pos.x:
                self.right()
            elif self.ai.pos.x > pos.x:
                self.left()
            elif self.ai.pos.y < pos.y:
                self.down()
            elif self.ai.pos.y > pos.y:
                self.up()
            if look:
                self.on_look()

    def go_where(self, resource: zp.ObjectType, look: bool = False, recursion: int | None = None) -> bool:
        if self.ai.world[self.ai.pos].objects[resource] > 0:
            return True
        i: int = 1
        x: int = self.ai.pos.x
        y: int = self.ai.pos.y
        if self.ai.world.size.width > self.ai.world.size.height:
            size: int = self.ai.world.size.width
        else:
            size: int = self.ai.world.size.height
        for _ in range((size // 2) + 1):
            x += i
            if self.ai.world[(y, x)].objects[resource] > 0:
                self.goto(zp.Pos(y, x), look)
                return True
            y -= i
            if self.ai.world[(y, x)].objects[resource] > 0:
                self.goto(zp.Pos(y, x), look)
                return True
            i += 1
            x -= i
            if self.ai.world[(y, x)].objects[resource] > 0:
                self.goto(zp.Pos(y, x), look)
                return True
            y += i
            if self.ai.world[(y, x)].objects[resource] > 0:
                self.goto(zp.Pos(y, x), look)
                return True
            i += 1
        if look and recursion is not None and recursion > 0:
            self.forward()
            self.on_look()
            return self.go_where(resource, look, recursion - 1)
        return False

    def _cartography_get_move(self, start: zp.Pos, end: zp.Pos) -> list[int, int, int]:
        zone_height: int = start.y - end.y
        zone_width: int = end.x - start.x
        triangle_height: int = self.ai.level + 1

        nb_triangle_height: int = zone_height // triangle_height
        nb_triangle_width: int = zone_width // self.ai.level

        nb_up: int = (nb_triangle_width // 2) - 1
        nb_down: int = nb_up
        if nb_triangle_width % 2 == 1:
            nb_up += 1
        return [nb_triangle_height, nb_up, nb_down]

    def _cartography_next_column(self) -> None:
        for _ in range(self.ai.level + 1):
            self.right()

    def _cartography_up_column(self, ops: list[int, int, int]) -> None:
        self.turn(zp.Direction.N)
        self.on_look()
        for idx in range(ops[0]):
            for _ in range(self.ai.level + 1):
                self.up()
            self.on_look()
            if idx == ops[0] - 1:
                self.up()

    def _cartography_down_column(self, ops: list[int, int, int]) -> None:
        self.turn(zp.Direction.S)
        self.on_look()
        for idx in range(ops[0]):
            for _ in range(self.ai.level + 1):
                self.down()
            self.on_look()
            if idx == ops[0] - 1:
                self.down()

    def cartography(self, start: zp.Pos, end: zp.Pos) -> None:
        if start == end:
            self.on_look()
            return
        if start.x > end.x:
            start.x, end.x = end.x, start.x
        if start.y < end.y:
            start.y, end.y = end.y, start.y
        if start.x < 0 or start.y < 0 or end.x > self.ai.world.size.width - 1 or end.y > self.ai.world.size.height - 1:
            raise ValueError("start and end must be inside the map")

        print("cartography from", start, "to", end)
        self.goto(start)
        self.turn(zp.Direction.N)

        ops: list[int, int, int] = self._cartography_get_move(start, end)
        while ops[1] > 0:
            self._cartography_up_column(ops)
            ops[1] -= 1
            if ops[2] > 0:
                self._cartography_next_column()
                self._cartography_down_column(ops)
                ops[2] -= 1
                if ops[1] > 0:
                    self._cartography_next_column()
        print("cartography done")

    def worker(self) -> None:
        print("worker")
        while self.ai.master_id is None:
            self.ai.recv(False)
            self.ai.exec_buffer()
        print("master found")
        while not self.ai.master_found:
            self.find_master()
        self.drop_all()
        while True:
            if self.ai.level > 5 and self.ai.id > 4:
                self.goto(zp.Pos(5, 5))
                self.die()
            self.change_status(zp.Status.MOVING)
            self.change_status(zp.Status.SEARCHING)
            self.goto(zp.Pos(random.randint(0, self.ai.world.size.height - 1),
                             random.randint(0, self.ai.world.size.width - 1)), True)
            self.change_status()
            self.goto(zp.Pos(5, 5), True)
            self.drop_all()
            self.change_status()

    def die(self) -> None:
        self.ai.check_inventory()
        self.drop_all(False)
        try:
            self.forward()
            self.forward()
            while True:
                self.ai.check_inventory()
        except TimeoutError:
            print("Successfully died !")
            exit(0)

    def find_master(self) -> None:
        while not self.ai.run_message("ping master", self.ai.master_id):
            self.ai.recv(False)
        if self.master_direction == zp.Direction.TOP:
            self.forward()
            self.ai.broadcast(self.ai.msg_handler["orientation master"].to_json(False, self.ai.master_id))
            while not self.ai.run_message("orientation master", self.ai.master_id):
                self.ai.recv(False)
            while not self.ai.run_message("ping master", self.ai.master_id):
                self.ai.recv(False)
            self.turn(self.master_direction)
            self.forward()
            self.turn(zp.Direction.N)
            self.ai._pos = zp.Pos(5, 5)
            self.ai.master_found = True
            if self.ai.id > 6:
                self.die()
            if self.ai.id == 0:
                self.queen()

            return

        self.turn(self.master_direction)
        self.forward()

    def drop_all(self, survive: bool = True) -> None:
        for resource in self.ai.inventory:
            if resource == zp.ObjectType.PLAYER:
                continue
            for _ in range(self.ai.inventory[resource]):
                if resource == zp.ObjectType.FOOD and (
                        self.ai.inventory.food <= (8 if self.ai.elevation else 5) or self.ai.elevation) and survive:
                    continue
                self.ai.set(resource)

    def take_resources(self, resources: zp.Resources) -> bool:
        for resource in resources:
            if resource == zp.ObjectType.PLAYER:
                continue
            while self.ai.inventory[resource] < resources[resource]:
                if not self.take(resource):
                    return False
        return True

    def can_elevate(self) -> bool:
        print(">>> can elevate ?")
        self.ai.look()
        resources: zp.Resources = self._queen_get_resources()
        print("world:", self.ai.world[self.ai.pos].objects)
        print("level:", resources)

        for resource in self.ai.world[self.ai.pos].objects:
            if resource == zp.ObjectType.PLAYER:
                continue
            if self.ai.world[self.ai.pos].objects[resource] < resources[resource]:
                return False
        return True

    @property
    def nb_players(self) -> int:
        if self.ai.level < 6:
            return 8
        return 6

    def _queen_get_resources(self) -> zp.Resources:
        resources: list[zp.Resources] = [
            zp.Resources(0, 8, 0, 0, 0, 0, 0, 0),
            zp.Resources(0, 4, 4, 4, 0, 0, 0, 0),
            zp.Resources(0, 8, 4, 0, 0, 8, 0, 0),
            zp.Resources(0, 2, 2, 4, 0, 2, 0, 0),
            zp.Resources(0, 1, 2, 3, 0, 1, 0, 0),
            zp.Resources(0, 2, 2, 2, 2, 2, 1, 0),
        ]
        return resources[self.ai.level - 1]

    def _queen_elevate_1(self) -> None:
        for current_id in range(1, 7):
            self.ai.broadcast(
                self.ai.msg_handler["incantation"].to_json(False, current_id, direction=zp.Direction.TOP, carrier=True))
        self.ai.broadcast(
            self.ai.msg_handler["incantation"].to_json(False, self.ai.master_id, direction=zp.Direction.TOP,
                                                       carrier=True))
        self.ai.incantation(False)

    def _queen_elevate_2_3(self) -> None:
        self.ai.broadcast(
            self.ai.msg_handler["incantation"].to_json(False, 1, direction=zp.Direction.N, carrier=True))
        self.ai.broadcast(
            self.ai.msg_handler["incantation"].to_json(False, 2, direction=zp.Direction.N, carrier=False))

        self.ai.broadcast(
            self.ai.msg_handler["incantation"].to_json(False, 3, direction=zp.Direction.E, carrier=True))
        self.ai.broadcast(
            self.ai.msg_handler["incantation"].to_json(False, 4, direction=zp.Direction.E, carrier=False))

        self.ai.broadcast(
            self.ai.msg_handler["incantation"].to_json(False, 5, direction=zp.Direction.W, carrier=True))
        self.ai.broadcast(
            self.ai.msg_handler["incantation"].to_json(False, 6, direction=zp.Direction.W, carrier=False))

        self.ai.broadcast(
            self.ai.msg_handler["incantation"].to_json(False, self.ai.master_id, direction=zp.Direction.TOP,
                                                       carrier=False))
        self.ai.incantation(False)

    def _queen_elevate_4_5(self) -> None:
        self.ai.broadcast(
            self.ai.msg_handler["incantation"].to_json(False, 1, direction=zp.Direction.N, carrier=True))
        self.ai.broadcast(
            self.ai.msg_handler["incantation"].to_json(False, 2, direction=zp.Direction.N, carrier=False))
        self.ai.broadcast(
            self.ai.msg_handler["incantation"].to_json(False, 3, direction=zp.Direction.N, carrier=False))
        self.ai.broadcast(
            self.ai.msg_handler["incantation"].to_json(False, 4, direction=zp.Direction.N, carrier=False))

        self.ai.broadcast(
            self.ai.msg_handler["incantation"].to_json(False, 5, direction=zp.Direction.TOP, carrier=False))
        self.ai.broadcast(
            self.ai.msg_handler["incantation"].to_json(False, 6, direction=zp.Direction.TOP, carrier=False))
        self.ai.broadcast(
            self.ai.msg_handler["incantation"].to_json(False, self.ai.master_id, direction=zp.Direction.TOP,
                                                       carrier=False))
        self.ai.incantation(False)

    def queen_elevate(self) -> None:
        self.ai.broadcast(self.ai.msg_handler["elevate"].to_json(False))
        self.ai.look()
        while self.ai.world[self.ai.pos].objects[zp.ObjectType.PLAYER] < self.nb_players - 1:
            print("waiting for", self.nb_players, "players",
                  self.ai.world[self.ai.pos].objects[zp.ObjectType.PLAYER] + 1, "currently")
            self.ai.look()
        fns = [self._queen_elevate_1, self._queen_elevate_2_3, self._queen_elevate_2_3, self._queen_elevate_4_5,
               self._queen_elevate_4_5]
        fns[self.ai.level - 1]()

    def queen(self) -> None:
        print("queen")
        self.ai.role = zp.Role.QUEEN
        self.ai.food_wanted = 4
        cycle: int = ai.INCANTATION_RATE
        incantation_lock: bool = False
        while True:
            self.change_status(zp.Status.NOTHING)
            print(">>> QUEEN")
            self.master_anti_starving()
            if incantation_lock:
                print(f">>> Incantation locked for {cycle} cycles")
                cycle -= 1
                if cycle <= 0:
                    incantation_lock = False
                    cycle = ai.INCANTATION_RATE
            if self._all_spawned and self.can_elevate():
                if not incantation_lock:
                    self.queen_elevate()
                    incantation_lock = True
            if not self._all_spawned and self._fork and self.ai.connect_nbr() == 0:
                self.ai.fork()
            if self.ai.inventory.food < self.food_wanted:
                self.take(zp.ObjectType.FOOD)
            self.change_status()

    def master_anti_starving(self, check_inventory: bool = True) -> None:
        if check_inventory:
            self.ai.check_inventory()
            self.ai.look()
            self.master_anti_starving(False)
            return
        if self.ai.inventory.food > 4 or self.ai.world[self.ai.pos].objects[zp.ObjectType.FOOD] > 0:
            return
        self.ai.broadcast(self.ai.msg_handler["starving"].to_json(False))

    def run(self) -> None:
        # TODO: check if map already cartography
        # TODO: check if master
        # TODO: check if lonely

        if self.ai.role == zp.Role.WORKER:
            self.worker()
            return
        self.change_status(zp.Status.NOTHING)
        while True:
            print(utils.WHITE)
            self.master_anti_starving()
            self.ai.broadcast(self.ai.msg_handler["ping master"].to_json(False))
            if self.ai.inventory.food > self.food_wanted:
                self.ai.check_inventory()
                continue
            self.take(zp.ObjectType.FOOD)


@dataclass
class HANDLER:
    _name: str
    _recv: Callable
    _send: Callable
    _msg: 'Message'

    def to_json(self, ans: bool = False, to: int | None = None, *args, **kwargs) -> str | None:
        data: str | None = self._send(self._msg, *args, **kwargs)
        if data is None:
            return None
        if data == "null":
            data = None
        res: dict = {
            "from": self._msg.brain.ai.id,
            "to": to,
            "ans": ans,
            "what": self._name,
            "data": data
        }
        return json.dumps(res)

    def __call__(self, direction: zp.Direction, msg: dict) -> None:
        data = msg["data"]
        try:
            data = json.loads(data)
            msg["data"] = data
        except json.JSONDecodeError:
            pass
        return self._recv(self._msg, direction, msg)

    def __init__(self, msg: 'Message', name: str, recv_handler: Callable, send_handler: Callable):
        self._name = name
        self._msg = msg
        self._recv = recv_handler
        self._send = send_handler


class Message:
    HANDLERS: dict[str, HANDLER] = {}
    brain: Brain

    def __init__(self, br: Brain):
        self.brain = br

        self["bootstrap master"] = (recv_bootstrap_master, send_bootstrap_master)
        self["new master"] = (recv_new_master, send_new_master)
        self["ping master"] = (recv_ping_master, send_ping_master)
        self["orientation master"] = (recv_orientation_master, send_orientation_master)
        self["elevate"] = (recv_elevate, send_elevate)
        self["incantation"] = (recv_incantation, send_incantation)
        self["starving"] = (recv_starving, send_starving)
        self["all spawned"] = (recv_all_spawned, send_all_spawned)

    def __getitem__(self, item):
        return self.HANDLERS[item]

    def __setitem__(self, key, value: tuple[Callable, Callable]):
        self.HANDLERS[key] = HANDLER(self, key, value[0], value[1])


def recv_bootstrap_master(msg: Message, direction: zp.Direction, data: dict) -> None:
    if msg.brain.ai.role == zp.Role.WORKER and msg.brain.ai.master_id is not None:
        return
    if data["ans"]:
        if msg.brain.ai.role is None:
            if data["data"] == "no master":  # FIXME: doesn't work
                msg.brain.ai.role = zp.Role.MASTER
                msg.brain.ai.master_id = msg.brain.ai.id
                msg.brain.ai.broadcast(msg["new master"].to_json(True))
    else:
        if msg.brain.ai.role == zp.Role.MASTER:
            msg.brain.ai.broadcast(msg["bootstrap master"].to_json(True, int(data["from"])))
        else:
            msg.brain.ai.broadcast(msg["bootstrap master"].to_json(True, int(data["from"])))


def send_bootstrap_master(msg: Message, *args, **kwargs) -> str | None:
    if msg.brain.ai.role == zp.Role.MASTER:
        return str(msg.brain.ai.id)
    if msg.brain.ai.master_id is None:
        return "no master"
    # return str(msg.brain.ai.master_id)


def recv_new_master(msg: Message, direction: zp.Direction, data: dict) -> None:
    if not data["ans"]:
        return
    if msg.brain.ai.role == zp.Role.MASTER:
        raise ValueError("already master")
    msg.brain.ai.role = zp.Role.WORKER
    msg.brain.ai.master_id = int(data["data"])


def send_new_master(msg: Message, *args, **kwargs) -> str | None:
    if msg.brain.ai.role == zp.Role.MASTER:
        return str(msg.brain.ai.id)


def recv_ping_master(msg: Message, direction: zp.Direction, data: dict) -> None:
    if msg.brain.ai.role != zp.Role.WORKER:
        return
    if direction == zp.Direction.TOP:
        msg.brain.master_direction = zp.Direction.TOP
        return
    path: int = (direction.value if direction.value % 2 != 0 else direction.value + 1)
    if msg.brain.ai.direction == zp.Direction.N:
        pass
    elif msg.brain.ai.direction == zp.Direction.W:
        path += 2
    elif msg.brain.ai.direction == zp.Direction.S:
        path += 4
    elif msg.brain.ai.direction == zp.Direction.E:
        path += 6
    while path > 8:
        path -= 8
    msg.brain.master_direction = zp.Direction(path)


def send_ping_master(msg: Message, *args, **kwargs) -> str | None:
    return "ping"


def recv_orientation_master(msg: Message, direction: zp.Direction, data: dict) -> None:
    if data["ans"] and msg.brain.ai.role == zp.Role.WORKER:
        new_direction: zp.Direction = zp.Direction(int(data["data"]["direction"]))
        if new_direction == zp.Direction.N:
            pass
        elif new_direction == zp.Direction.W:
            msg.brain.ai.right()
        elif new_direction == zp.Direction.S:
            msg.brain.ai.right()
            msg.brain.ai.right()
        elif new_direction == zp.Direction.E:
            msg.brain.ai.left()
        msg.brain.ai._direction = zp.Direction.N
        msg.brain.ai.id = int(data["data"]["id"])
        print("new id:", msg.brain.ai.id)
    elif msg.brain.ai.role == zp.Role.MASTER:
        msg.brain.ai.broadcast(msg["orientation master"].to_json(True, int(data["from"]), direction=direction))


def send_orientation_master(msg: Message, *args, **kwargs) -> str | None:
    if msg.brain.ai.role == zp.Role.MASTER:
        if "direction" in kwargs:
            if len(msg.brain.players) > 5:
                msg.brain.ai.broadcast(msg["all spawned"].to_json(True, 0))
            msg.brain.players.append(len(msg.brain.players))
            return json.dumps({
                "direction": str(int(kwargs["direction"].value)),
                "id": msg.brain.players[-1]
            })
    else:
        return "?"


def recv_elevate(msg: Message, direction: zp.Direction, data: dict) -> None:
    if msg.brain.ai.role == zp.Role.MASTER or msg.brain.ai.role == zp.Role.QUEEN or data["ans"]:
        return
    while not msg.brain.ai.master_found:
        msg.brain.find_master()
    msg.brain.ai.elevation = True
    msg.brain.change_status(zp.Status.MOVING)
    current_lvl = msg.brain.ai.level
    msg.brain.goto(zp.Pos(5, 5))
    print("===========================on master===========================")
    msg.brain.drop_all()
    while msg.brain.ai.level == current_lvl:
        print(utils.CYAN)
        msg.brain.ai.check_inventory()
        if msg.brain.ai.inventory.food < 4:
            msg.brain.take(zp.ObjectType.FOOD)
    msg.brain.change_status()


def send_elevate(msg: Message, *args, **kwargs) -> str | None:
    return "ready"


def recv_incantation(msg: Message, direction: zp.Direction, data: dict, recursion: int = 3) -> None:
    if msg.brain.ai.role is None or data["ans"]:
        return
    print(utils.BLUE)
    direction: zp.Direction = zp.Direction(int(data["data"]["direction"]))
    carrier: bool = bool(data["data"]["carrier"])
    while msg.brain.ai.inventory.food < 6:
        if not msg.brain.ai.take(zp.ObjectType.FOOD):
            break
    if carrier and direction != zp.Direction.TOP:
        msg.brain.take_resources(ai.OBJECTIVES[msg.brain.ai.level - 1])
    if direction != zp.Direction.TOP:
        msg.brain.turn(direction)
        msg.brain.forward()
    msg.brain.drop_all()
    if carrier:
        # msg.brain.ai.incantation(direction != zp.Direction.TOP) # FIXME: drop item not working
        # below is a fix to that (we just drop before incantation)
        msg.brain.ai.look()
        while msg.brain.ai.world[msg.brain.ai.pos].objects.player < ai.OBJECTIVES[msg.brain.ai.level - 1].player:
            msg.brain.ai.look()
        try:
            if not msg.brain.ai.incantation(False):
                raise ValueError("incantation failed")
        except ValueError:
            if recursion > 0:
                print("incantation failed, retrying...")
                msg.brain.goto(zp.Pos(5, 5))
                recv_incantation(msg, direction, data, recursion - 1)
            else:
                raise ValueError("incantation failed, aborting...")


def send_incantation(msg: Message, *args, **kwargs) -> str | None:
    direction: zp.Direction = kwargs["direction"]
    carrier: bool = kwargs["carrier"]
    return json.dumps({
        "direction": direction.value,
        "carrier": carrier
    })


def recv_starving(msg: Message, direction: zp.Direction, data: dict) -> None:
    if msg.brain.ai.role != zp.Role.WORKER:
        return
    if msg.brain.ai.inventory.food < 6:
        return
    msg.brain.goto(zp.Pos(5, 5))
    msg.brain.drop_all()


def send_starving(msg: Message, *args, **kwargs) -> str | None:
    return "starving"


def recv_all_spawned(msg: Message, direction: zp.Direction, data: dict) -> None:
    if msg.brain.ai.role != zp.Role.QUEEN:
        return
    msg.brain._all_spawned = True


def send_all_spawned(msg: Message, *args, **kwargs) -> str | None:
    return "all spawned"
