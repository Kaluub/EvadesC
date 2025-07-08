import os.path
import struct
import yaml
from enum import IntEnum
from io import BufferedWriter

class ComponentFlags(IntEnum):
    HAS_BACKGROUND_COLOR = 0
    HAS_TEXTURE = 1
    HAS_NAME = 2
    HAS_SPAWNER = 3
    HAS_TRANSLATE = 4
    USES_PREVIOUS_AREA_DIMENSIONS = 5

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

def are_area_dimensions_shared(area: dict, previous_area: dict):
    zones: list[dict] = area["zones"]
    previous_zones: list[dict] = previous_area["zones"]

    if len(zones) != len(previous_zones):
        return False
    
    for i in range(len(zones)):
        if (
            zones[i]["width"] != previous_zones[i]["width"]
            or zones[i]["height"] != previous_zones[i]["height"]
            or zones[i]["x"] != previous_zones[i]["x"]
            or zones[i]["y"] != previous_zones[i]["y"]
            or zones[i]["type"] != previous_zones[i]["type"]
        ):
            return False
        translate = zones[i].get("translate", None)
        previous_translate = previous_zones[i].get("translate", None)
        if translate is not None and (
            translate["x"] != previous_translate["x"]
            or translate["y"] != previous_translate["y"]
        ):
            return False

    return True


def handle_common_properties(
        target: dict,
        previous_properties: dict | None,
        out: BufferedWriter,
        previous_area: dict = None,
    ):
    flags = 0
    background_color = 0
    texture = None
    only_write_spawner = False
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
    translate = target.get("translate", None)
    if translate is not None:
        flags |= 1 << ComponentFlags.HAS_TRANSLATE
    if previous_area is not None:
        if are_area_dimensions_shared(target, previous_area):
            flags |= 1 << ComponentFlags.USES_PREVIOUS_AREA_DIMENSIONS
            only_write_spawner = True
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
    if flags & (1 << ComponentFlags.HAS_TRANSLATE):
        x: int = translate["x"]
        y: int = translate["y"]
        write_greedy_int(x, out)
        write_greedy_int(y, out)
    return {"background_color": background_color, "texture": texture, "only_write_spawner": only_write_spawner}

def write_spawners(spawners: list, out: BufferedWriter):
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
        write_greedy_uint(spawner.get("count", 1), out)
        write_greedy_uint(spawner["radius"], out)

def write_greedy_uint(number: int, out: BufferedWriter):
    assert number >= 0 and number < 1<<30
    extra_bytes = 0
    if number >= 1<<22:
        extra_bytes = 3
    elif number >= 1<<14:
        extra_bytes = 2
    elif number >= 1<<6:
        extra_bytes = 1

    data = ((number & 0xFFFFFFC0) << 2) | (extra_bytes << 6) | (number & 0x3F)
    out.write(data.to_bytes(extra_bytes + 1, "little"))

def write_greedy_int(number: int, out: BufferedWriter):
    assert number > -(1<<29) and number < 1<<29
    extra_bytes = 0
    if number >= 1<<21 or number <= -(1<<21):
        extra_bytes = 3
    elif number >= 1<<13 or number <= -(1<<13):
        extra_bytes = 2
    elif number >= 1<<5 or number <= -(1<<5):
        extra_bytes = 1

    sign = 0
    if number < 0:
        sign = 1
        number *= -1
    data = ((number & 0xFFFFFFE0) << 3) | (extra_bytes << 6) | (sign << 5) | (number & 0x1F)
    out.write(data.to_bytes(extra_bytes + 1, "little"))

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
    "wavering": 128,
    "cursed": 129,
    "expander": 130,
    "silence": 131,
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

def main():
    out = open("maps/world.bin", "wb")

    total_areas = 0
    same_areas = 0

    directory = os.environ.get("WORLD_DIR") or "maps/definitions"
    print(f"Using directory '{directory}'.")

    with open(f"{directory}/world.yaml", encoding="utf-8") as world_file:
        world = yaml.load(world_file, yaml.CLoader)
        spawn_name = f"{world['spawn']}\0".encode("ascii")
        out.write(len(spawn_name).to_bytes(1, "little"))
        out.write(spawn_name)
        out.write(len(world["regions"]).to_bytes(1, "little"))

        for region_meta in world["regions"]:
            region_x = region_meta["x"]
            region_y = region_meta["y"]
            with open(f"{directory}/{region_meta['file']}") as region_file:
                region = yaml.load(region_file, yaml.CLoader)
                region_properties = handle_common_properties(region, None, out)
                
                previous_area = None

                # Write out areas
                out.write(len(region["areas"]).to_bytes(2, "little"))
                area_state = {"var x": region_x, "var y": region_y}
                for area in region["areas"]:
                    # Write out area absolute position
                    area_x = parse_variable(area["x"], area_state)
                    area_y = parse_variable(area["y"], area_state)
                    area_width = 0
                    area_height = 0

                    #assert area_x % 16 == 0, f"{region["name"]}: {area_x}"
                    #assert area_y % 16 == 0, f"{region["name"]}: {area_y}"

                    if previous_area is None:
                        write_greedy_int(area_x, out)
                        write_greedy_int(area_y, out)
                    else:
                        write_greedy_int((area_x - area_state["last_x"]), out)
                        write_greedy_int((area_y - area_state["last_y"]), out)

                    area_properties = handle_common_properties(area, region_properties, out, previous_area=previous_area)

                    if area_properties["only_write_spawner"]:
                        spawners = None
                        for zone in area["zones"]:
                            s = zone.get("spawner", None)
                            if s is not None:
                                spawners = s
                                break
                        write_spawners(spawners, out)
                        area_width = area_state["last_width"]
                        area_height = area_state["last_height"]
                        same_areas += 1
                    else:
                        # Write out zones
                        out.write(len(area["zones"]).to_bytes(1, "little"))
                        zone_state = {}
                        for zone in area["zones"]:
                            zone_type = zone_types[zone["type"]]
                            zone_x = parse_variable(zone["x"], zone_state)
                            zone_y = parse_variable(zone["y"], zone_state)
                            zone_width = parse_variable(zone["width"], zone_state)
                            zone_height = parse_variable(zone["height"], zone_state)

                            # Write out zone dimensions
                            out.write(zone_type.to_bytes(1, "little"))
                            assert zone_x % 16 == 0, f"{region["name"]}: {zone_x}"
                            assert zone_y % 16 == 0, f"{region["name"]}: {zone_y}"
                            assert zone_width % 16 == 0, f"{region["name"]}: {zone_width}"
                            assert zone_height % 16 == 0, f"{region["name"]}: {zone_height}"
                            write_greedy_int(zone_x // 16, out)
                            write_greedy_int(zone_y // 16, out)
                            write_greedy_uint(zone_width // 16, out)
                            write_greedy_uint(zone_height // 16, out)

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
                    previous_area = area
                    total_areas += 1

    out.close()
    print(f"Wrote world.bin -> {os.path.getsize('maps/world.bin') / 1024:.3f} KiB.")
    print(f"Areas written: {total_areas}. Same area shapes: {same_areas}. ({same_areas/total_areas*100:.3f}%)")

main()