import json

file = open("scripts/colors.json", "r")
out: dict[str, dict] = json.load(file)

for [k, v] in out.items():
    color: str = v["color"]
    if color.startswith("rgb"):
        segments = color.split("(")[1].split(")")[0].split(",")
        color = (int(segments[0].strip()) << 24) | (int(segments[1].strip()) << 16) | (int(segments[2].strip()) << 8)
        if len(segments) > 3:
            color |= int(float(segments[3]) * 255)
        else:
            color |= 255
        color = hex(color)
    else:
        color = color.replace("#", "0x")
        if len(color) != 9:
            color = color + "ff"
    print(f"{color}, // {k}")