#include "map.h"
#include <malloc.h>

void load_map(Map* map, FILE* file) {
    // Spawn region name.
    uint8_t spawn_region_length = 0;
    fread(&spawn_region_length, sizeof(spawn_region_length), 1, file);
    map->spawn_region = (char *) malloc(spawn_region_length);
    fread(map->spawn_region, 1, spawn_region_length, file);

    // Regions.
    fread(&map->region_count, sizeof(map->region_count), 1, file);
    if (map->region_count <= 0) {
        map->regions = NULL;
        return;
    }
    map->regions = (Region*) malloc(sizeof(Region) * map->region_count);

    for (int region_index = 0; region_index < map->region_count; region_index++) {
        Region* region = map->regions + region_index;
        // Region name.
        uint8_t region_name_length = 0;
        fread(&region_name_length, sizeof(region_name_length), 1, file);
        region->region_name = (char *) malloc(region_name_length);
        fread(region->region_name, 1, region_name_length, file);

        // Areas.
        fread(&region->area_count, sizeof(region->area_count), 1, file);
        if (region->area_count <= 0) {
            region->areas = NULL;
            continue;
        }
        region->areas = (Area*) malloc(sizeof(Area) * region->area_count);

        for (int area_index = 0; area_index < region->area_count; area_index++) {
            Area* area = region->areas + area_index;
            fread(&area->x, sizeof(area->x), 1, file);
            fread(&area->y, sizeof(area->y), 1, file);
            area->width = 0;
            area->height = 0;
            
            // Zones.
            fread(&area->zone_count, sizeof(area->zone_count), 1, file);
            if (area->zone_count <= 0) {
                area->zones = NULL;
                continue;
            }
            area->zones = (Zone*) malloc(sizeof(Zone) * area->zone_count);
            for (int zone_index = 0; zone_index < area->zone_count; zone_index++) {
                Zone* zone = area->zones + zone_index;
                fread(&zone->x, sizeof(zone->x), 1, file);
                fread(&zone->y, sizeof(zone->y), 1, file);
                fread(&zone->width, sizeof(zone->width), 1, file);
                fread(&zone->height, sizeof(zone->height), 1, file);
                fread(&zone->background_color, sizeof(zone->background_color), 1, file);
                fread(&zone->type, sizeof(zone->type), 1, file);

                if (area->width < (zone->x - area->x) + zone->width) {
                    area->width = (zone->x - area->x) + zone->width;
                }
                if (area->height < (zone->y - area->y) + zone->height) {
                    area->height = (zone->y - area->y) + zone->height;
                }
            }
        }
    }
}

void destroy_map(Map* map) {
    if (map->spawn_region != NULL) {
        free(map->spawn_region);
    }
    if (map->regions == NULL) {
        return;
    }
    for (int region_index = 0; region_index < map->region_count; region_index++) {
        Region* region = map->regions + region_index;
        if (region->region_name != NULL) {
            free(region->region_name);
        }
        if (region->areas == NULL) {
            continue;
        }
        for (int area_index = 0; area_index < region->area_count; area_index++) {
            Area* area = region->areas + area_index;
            if (area->zones == NULL) {
                continue;
            }
            free(area->zones);
        }
        free(region->areas);
    }
    free(map->regions);
    map->spawn_region = NULL;
    map->regions = NULL;
    map->region_count = 0;
}