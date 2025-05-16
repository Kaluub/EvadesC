import os.path
import struct
import yaml
from enum import IntEnum

class ComponentFlags(IntEnum):
    HAS_BACKGROUND_COLOR = 0
    HAS_TEXTURE = 1
    HAS_NAME = 2
    HAS_SPAWNER = 3

def parse_variable(definition: str, state: dict[str, int]) -> int:
    value = definition
    if isinstance(value, int):
        return value
    offset = 0
    if "+" in definition:
        split = definition.split("+")
        value = split[0].strip()
        offset = int(split[1].strip())
    if "-" in definition:
        split = definition.split("-")
        value = split[0].strip()
        offset = -int(split[1].strip())
    if value in state:
        value = state.get(value)
    else:
        value = int(value)
    return value + offset

def get_color(properties: dict) -> int:
    vec: list[int] = properties["background_color"]
    return vec[0] << 24 | vec[1] << 16 | vec[2] << 8 | vec[3]

def handle_common_properties(target: dict, previous_properties: dict | None, out):
    flags = 0
    background_color = 0
    texture = None
    properties = target.get("properties", None)
    if properties is not None:
        if "background_color" in properties:
            background_color = get_color(properties)
            if previous_properties is None or previous_properties["background_color"] != background_color:
                flags |= 1 << ComponentFlags.HAS_BACKGROUND_COLOR
        if "texture" in properties:
            texture = textures[properties["texture"]]
            if previous_properties is None or previous_properties["texture"] != texture:
                flags |= 1 << ComponentFlags.HAS_TEXTURE
    name = target.get("name", None)
    if name is not None:
        flags |= 1 << ComponentFlags.HAS_NAME
    spawners = target.get("spawner", None)
    if spawners is not None:
        flags |= 1 << ComponentFlags.HAS_SPAWNER
    out.write(flags.to_bytes(1, "little"))
    if flags & (1 << ComponentFlags.HAS_BACKGROUND_COLOR):
        out.write(background_color.to_bytes(4, "little"))
    if flags & (1 << ComponentFlags.HAS_TEXTURE):
        out.write(texture.to_bytes(1, "little"))
    if flags & (1 << ComponentFlags.HAS_NAME):
        encoded_name = f"{name}\0".replace("\u200b", "").replace("\u221e", "inf").encode("ascii", errors="ignore")
        out.write(len(encoded_name).to_bytes(1, "little"))
        out.write(encoded_name)
    if flags & (1 << ComponentFlags.HAS_SPAWNER):
        write_spawners(spawners, out)
    return {"background_color": background_color, "texture": texture}

def write_spawners(spawners: list, out):
    out.write(len(spawners).to_bytes(1, "little"))
    for spawner in spawners:
        spawner_types = spawner["types"]
        out.write(len(spawner_types).to_bytes(1, "little"))
        for enemy_type in spawner_types:
            enemy_type_num = enemy_types.get(enemy_type, None)
            if enemy_type is None:
                raise ValueError(spawner_types, enemy_type)
            out.write(enemy_type_num.to_bytes(1, "little"))
        out.write(struct.pack("<f", spawner.get("speed", 0)))
        out.write(spawner.get("count", 1).to_bytes(2, "little"))
        out.write(spawner["radius"].to_bytes(2, "little"))
        
        
enemy_types = {
    "wall": 1,
    "normal": 2,
    "homing": 3,
    "homing_switch": 4,
    "dasher": 5,
    "dasher_switch": 6,
    "slowing": 7,
    "draining": 8,
    "gravity": 9,
    "repelling": 10,
    "turning": 11,
    "sizing": 12,
    "sniper": 13,
    "freezing": 14,
    "teleporting": 15,
    "wavy": 16,
    "wavy_switch": 17,
    "zigzag": 18,
    "zigzag_switch": 19,
    "confectioner": 20,
    "confectioner_switch": 21,
    "zoning": 22,
    "zoning_switch": 23,
    "spiral": 24,
    "spiral_switch": 25,
    "oscillating": 26,
    "oscillating_switch": 27,
    "switch": 28,
    "dorito": 29,
    "dorito_switch": 30,
    "penny": 31,
    "penny_switch": 32,
    "infinity": 33,
    "infinity_switch": 34,
    "liquid": 35,
    "icicle": 36,
    "slippery": 37,
    "ice_sniper": 38,
    "disabling": 39,
    "experience_drain": 40,
    "enlarging": 41,
    "speed_sniper": 42,
    "regen_sniper": 43,
    "radiating_bullets": 44,
    "immune": 45,
    "pumpkin": 46,
    "fake_pumpkin": 47,
    "tree": 48,
    "frost_giant": 49,
    "snowman": 50,
    "corrosive": 51,
    "toxic": 52,
    "corrosive_sniper": 53,
    "poison_sniper": 54,
    "magnetic_reduction": 55,
    "magnetic_nullification": 56,
    "positive_magnetic_sniper": 57,
    "negative_magnetic_sniper": 58,
    "residue": 59,
    "fire_trail": 60,
    "ice_ghost": 61,
    "poison_ghost": 62,
    "positive_magnetic_ghost": 63,
    "negative_magnetic_ghost": 64,
    "wind_ghost": 65,
    "lunging": 66,
    "lava": 67,
    "gravity_ghost": 68,
    "repelling_ghost": 69,
    "star": 70,
    "grass": 71,
    "seedling": 72,
    "flower": 73,
    "disabling_ghost": 74,
    "glowy": 75,
    "firefly": 76,
    "mist": 77,
    "phantom": 78,
    "cybot": 79,
    "eabot": 80,
    "wabot": 81,
    "fibot": 82,
    "aibot": 83,
    "wind_sniper": 84,
    "sand": 85,
    "sandrock": 86,
    "quicksand": 87,
    "crumbling": 88,
    "radar": 89,
    "barrier": 90,
    "speed_ghost": 91,
    "regen_ghost": 92,
    "cactus": 93,
    "cycling": 94,
    "icbot": 95,
    "elbot": 96,
    "plbot": 97,
    "mebot": 98,
    "libot": 99,
    "dabot": 100,
    "sparking": 101,
    "thunderbolt": 102,
    "static": 103,
    "electrical": 104,
    "prediction_sniper": 105,
    "ring_sniper": 106,
    "charging": 107,
    "reducing": 108,
    "lead_sniper": 109,
    "stalactite": 110,
    "blocking": 111,
    "force_sniper_a": 112,
    "force_sniper_b": 113,
    "wacky_wall": 114,
    "flaming": 115,
    "stumbling": 116,
    "disarming": 117,
    "lurching": 118,
    "infectious": 119,
    "mutating": 120,
    "vengeful_soul": 121,
    "lost_soul": 122,
    "blind": 123,
    "ninja_star_sniper": 124,
    "summoner": 125,
    "slasher": 126,
    "lotus_flower": 127,
}


zone_types = {
    "safe": 1,
    "active": 2,
    "exit": 3,
    "teleport": 4,
    "victory": 5,
    "removal": 6,
    "dummy": 7,
}

textures = {
    "leaves": 1,
}

out = open("maps/world.bin", "wb")
with open("maps/definitions/world.yaml", encoding="utf-8") as world_file:
    world = yaml.load(world_file, yaml.CLoader)
    spawn_name = f"{world['spawn']}\0".encode("ascii")
    out.write(len(spawn_name).to_bytes(1, "little"))
    out.write(spawn_name)
    out.write(len(world["regions"]).to_bytes(1, "little"))

    for region_meta in world["regions"]:
        region_x = region_meta["x"]
        region_y = region_meta["y"]
        with open(f"maps/definitions/{region_meta['file']}") as region_file:
            region = yaml.load(region_file, yaml.CLoader)
            region_properties = handle_common_properties(region, None, out)

            # Write out areas
            out.write(len(region["areas"]).to_bytes(2, "little"))
            area_state = {"var x": region_x, "var y": region_y}
            for area in region["areas"]:
                # Write out area absolute position
                area_x = parse_variable(area["x"], area_state)
                area_y = parse_variable(area["y"], area_state)
                area_width = 0
                area_height = 0
                out.write(area_x.to_bytes(4, "little", signed=True))
                out.write(area_y.to_bytes(4, "little", signed=True))

                area_properties = handle_common_properties(area, region_properties, out)

                # Write out zones
                out.write(len(area["zones"]).to_bytes(1, "little"))
                zone_state = {}
                for zone in area["zones"]:
                    zone_type = zone_types[zone["type"]]
                    zone_x = parse_variable(zone["x"], zone_state)
                    zone_y = parse_variable(zone["y"], zone_state)
                    zone_width = parse_variable(zone["width"], zone_state)
                    zone_height = parse_variable(zone["height"], zone_state)

                    # Write out zone absolute dimensions
                    out.write(zone_type.to_bytes(1, "little"))
                    out.write((area_x + zone_x).to_bytes(4, "little", signed=True))
                    out.write((area_y + zone_y).to_bytes(4, "little", signed=True))
                    out.write(zone_width.to_bytes(4, "little"))
                    out.write(zone_height.to_bytes(4, "little"))

                    handle_common_properties(zone, area_properties, out)

                    # Update area size
                    if zone_x + zone_width > area_width:
                        area_width = zone_x + zone_width
                    if zone_y + zone_height > area_height:
                        area_height = zone_y + zone_height
                    # Store previous zone states
                    zone_state["last_x"] = zone_x
                    zone_state["last_y"] = zone_y
                    zone_state["last_width"] = zone_width
                    zone_state["last_height"] = zone_height
                    zone_state["last_right"] = zone_x + zone_width
                    zone_state["last_bottom"] = zone_y + zone_height
                # Store previous area states
                area_state["last_x"] = area_x
                area_state["last_y"] = area_y
                area_state["last_width"] = area_width
                area_state["last_height"] = area_height
                area_state["last_right"] = area_x + area_width
                area_state["last_bottom"] = area_y + area_height

out.close()
print(f"Wrote world.bin -> {os.path.getsize('maps/world.bin') / 1024:.3f} KiB.")