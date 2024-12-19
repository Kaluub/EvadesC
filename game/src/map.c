#include "map.h"
#include <malloc.h>

void load_map(Map* map, FILE* file) {
    // Spawn region name.
    fread(&map->spawn_region_length, sizeof(map->spawn_region_length), 1, file);
    map->spawn_region = (char *) malloc(map->spawn_region_length);
    fread(map->spawn_region, 1, map->spawn_region_length, file);

    // Regions.
    fread(&map->region_count, sizeof(map->region_count), 1, file);
    if (map->region_count <= 0) {
        map->regions = NULL;
        return;
    }
    map->regions = (Region*) malloc(sizeof(Region) * map->region_count);

    for (int region_index = 0; region_index < map->region_count; region_index += 1) {
        Region* region = map->regions + region_index;
        // Region name.
        fread(&region->region_name_length, sizeof(region->region_name_length), 1, file);
        region->region_name = (char *) malloc(region->region_name_length);
        fread(region->region_name, 1, region->region_name_length, file);

        // Areas.
        fread(&region->area_count, sizeof(region->area_count), 1, file);
        if (region->area_count <= 0) {
            region->areas = NULL;
            continue;
        }
        region->areas = (Area*) malloc(sizeof(Area) * region->area_count);

        for (int area_index = 0; area_index < region->area_count; area_index += 1) {
            Area* area = region->areas + area_index;
            fread(&area->x, sizeof(area->x), 1, file);
            fread(&area->y, sizeof(area->y), 1, file);
            
            // Zones.
            fread(&area->zone_count, sizeof(area->zone_count), 1, file);
            if (area->zone_count <= 0) {
                area->zones = NULL;
                continue;
            }
            area->zones = (Zone*) malloc(sizeof(Zone) * area->zone_count);
            for (int zone_index = 0; zone_index < area->zone_count; zone_index += 1) {
                Zone* zone = area->zones + zone_index;
                fread(&zone->type, sizeof(zone->type), 1, file);
                fread(&zone->x, sizeof(zone->x), 1, file);
                fread(&zone->y, sizeof(zone->y), 1, file);
                fread(&zone->width, sizeof(zone->width), 1, file);
                fread(&zone->height, sizeof(zone->height), 1, file);
            }
        }
    }
}