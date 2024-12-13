#include "binary_map.h"
#include <stdlib.h>
#include <time.h>

void print_random_area(struct BinaryMap* map);

int main() {
    struct BinaryMap map;
    FILE* file = fopen("maps/world.bin", "rb");

    load_map(&map, file);
    printf("Loaded spawn region name: %s\nLoaded regions: %d\n", map.spawn_region, map.region_count);

    srand(time(NULL));
    for (int i = 0; i < 6; i += 1) {
        print_random_area(&map);
    }

    return 0;
}

void print_random_area(struct BinaryMap* map) {
    int region_index = rand() % map->region_count;
    struct BinaryRegion region = map->regions[region_index];
    int area_index = rand() % region.area_count;
    struct BinaryArea area = region.areas[area_index];
    printf("Area %d of %s:\n- %d zones\n- x, y = (%d, %d)\n Zones:\n", area_index, region.region_name, area.zone_count, area.x, area.y);
    for (int zone_index = 0; zone_index < area.zone_count; zone_index += 1) {
        struct BinaryZone zone = area.zones[zone_index];
        printf("  - zone %d with type %d and absolute rect (%d, %d, %d, %d)\n", zone_index, zone.type, zone.x, zone.y, zone.width, zone.height);
    }
}