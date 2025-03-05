import os.path
import yaml
from enum import IntEnum

class ComponentFlags(IntEnum):
    HAS_BACKGROUND_COLOR = 0
    HAS_TEXTURE = 1

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

def handle_common_properties(properties: dict | None, previous_properties: dict | None, out):
    flags = 0
    background_color = 0
    texture = None
    if properties is not None:
        if "background_color" in properties:
            background_color = get_color(properties)
            if previous_properties is None or previous_properties["background_color"] != background_color:
                flags |= 1 << ComponentFlags.HAS_BACKGROUND_COLOR
        if "texture" in properties:
            texture = textures[properties["texture"]]
            if previous_properties is None or previous_properties["texture"] != texture:
                flags |= 1 << ComponentFlags.HAS_TEXTURE
    out.write(flags.to_bytes(1, "little"))
    if flags & (1 << ComponentFlags.HAS_BACKGROUND_COLOR):
        out.write(background_color.to_bytes(4, "little"))
    if flags & (1 << ComponentFlags.HAS_TEXTURE):
        out.write(texture.to_bytes(1, "little"))
    return {"background_color": background_color, "texture": texture}

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
with open("maps/definitions/world.yaml") as world_file:
    world = yaml.load(world_file, yaml.CLoader)
    spawn_name = f"{world['spawn']}\0".encode("ascii")
    out.write(len(spawn_name).to_bytes(1, "little"))
    out.write(spawn_name)
    out.write(len(world["regions"]).to_bytes(2, "little"))

    for region_meta in world["regions"]:
        region_x = region_meta["x"]
        region_y = region_meta["y"]
        with open(f"maps/definitions/{region_meta['file']}") as region_file:
            region = yaml.load(region_file, yaml.CLoader)
            region_name = f"{region['name']}\0".encode("ascii")
            out.write(len(region_name).to_bytes(1, "little"))
            out.write(region_name)

            region_properties = handle_common_properties(region.get("properties", None), None, out)

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

                area_properties = handle_common_properties(area.get("properties", None), region_properties, out)

                # Write out zones
                out.write(len(area["zones"]).to_bytes(2, "little"))
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
                    out.write(zone_width.to_bytes(4, "little", signed=True))
                    out.write(zone_height.to_bytes(4, "little", signed=True))

                    handle_common_properties(zone.get("properties", None), area_properties, out)

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