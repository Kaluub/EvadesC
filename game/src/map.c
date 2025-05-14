#include "map.h"
#include <malloc.h>
#include <assert.h>

#define MAX_SPAWNER_COUNT 256

void load_map(Map* map, FILE* file) {
    // Spawn region name.
    uint8_t spawn_region_length = 0;
    fread(&spawn_region_length, sizeof(spawn_region_length), 1, file);
    map->spawn_region = (char*) malloc(spawn_region_length);
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

        // Region properties.
        uint8_t region_flags = 0;
        region->background_color = 0;
        region->texture = 0;
        fread(&region_flags, sizeof(region_flags), 1, file);
        if (region_flags & (1 << HAS_BACKGROUND_COLOR)) {
            fread(&region->background_color, sizeof(region->background_color), 1, file);
        }
        if (region_flags & (1 << HAS_TEXTURE)) {
            fread(&region->texture, sizeof(region->texture), 1, file);
        }

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

            // Cache for fast spawning algorithm.
            Spawner* spawners[MAX_SPAWNER_COUNT];
            Zone* spawner_zone_references[MAX_SPAWNER_COUNT];
            int spawner_list_size = 0;
            int enemy_count = 0;

            // Area properties.
            uint8_t area_flags = 0;
            area->background_color = region->background_color;
            area->texture = region->texture;
            fread(&area_flags, sizeof(area_flags), 1, file);
            if (area_flags & (1 << HAS_BACKGROUND_COLOR)) {
                fread(&area->background_color, sizeof(area->background_color), 1, file);
            }
            if (area_flags & (1 << HAS_TEXTURE)) {
                fread(&area->texture, sizeof(area->texture), 1, file);
            }
            
            // Zones.
            fread(&area->zone_count, sizeof(area->zone_count), 1, file);
            if (area->zone_count <= 0) {
                area->zones = NULL;
                continue;
            }
            area->zones = (Zone*) malloc(sizeof(Zone) * area->zone_count);
            for (int zone_index = 0; zone_index < area->zone_count; zone_index++) {
                Zone* zone = area->zones + zone_index;
                zone->spawner_count = 0;
                zone->spawners = NULL;
                fread(&zone->type, sizeof(zone->type), 1, file);
                fread(&zone->x, sizeof(zone->x), 1, file);
                fread(&zone->y, sizeof(zone->y), 1, file);
                fread(&zone->width, sizeof(zone->width), 1, file);
                fread(&zone->height, sizeof(zone->height), 1, file);
                
                // Zone properties.
                uint8_t zone_flags = 0;
                zone->background_color = area->background_color;
                zone->texture = area->texture;
                fread(&zone_flags, sizeof(zone_flags), 1, file);
                if (zone_flags & (1 << HAS_BACKGROUND_COLOR)) {
                    fread(&zone->background_color, sizeof(zone->background_color), 1, file);
                }
                if (zone_flags & (1 << HAS_TEXTURE)) {
                    fread(&zone->texture, sizeof(zone->texture), 1, file);
                }
                if (zone_flags & (1 << HAS_SPAWNER)) {
                    fread(&zone->spawner_count, sizeof(zone->spawner_count), 1, file);
                    zone->spawners = (Spawner*) malloc(sizeof(Spawner) * zone->spawner_count);
                    for (int spawner_index = 0; spawner_index < zone->spawner_count; spawner_index++) {
                        Spawner* spawner = zone->spawners + spawner_index;
                        fread(&spawner->enemy_type_count, sizeof(spawner->enemy_type_count), 1, file);
                        assert(spawner->enemy_type_count != 0);
                        spawner->enemy_types = (uint8_t*) malloc(sizeof(uint8_t) * spawner->enemy_type_count);
                        fread(spawner->enemy_types, sizeof(uint8_t), spawner->enemy_type_count, file);
                        fread(&spawner->speed, sizeof(spawner->speed), 1, file);
                        fread(&spawner->count, sizeof(spawner->count), 1, file);
                        fread(&spawner->radius, sizeof(spawner->radius), 1, file);
                        spawner_zone_references[spawner_list_size] = zone;
                        spawners[spawner_list_size++] = spawner;
                        enemy_count += spawner->count;
                    }
                    // Set active zone pointer.
                    // TODO: Support multiple active zones?
                    area->active_zone = zone;
                }

                if (area->width < (zone->x - area->x) + zone->width) {
                    area->width = (zone->x - area->x) + zone->width;
                }
                if (area->height < (zone->y - area->y) + zone->height) {
                    area->height = (zone->y - area->y) + zone->height;
                }
            }

            // Enemy spawning.
            assert(spawner_list_size < MAX_SPAWNER_COUNT);
            enemy_set_init(&area->enemy_set, enemy_count);
            for (int i = 0; i < spawner_list_size; i++) {
                Zone* zone = spawner_zone_references[i];
                Spawner* spawner = spawners[i];
                for (int j = 0; j < spawner->count; j++) {
                    Enemy* enemy = enemy_init(zone, spawner, j);
                    enemy_set_add(&area->enemy_set, enemy);
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
            for (int zone_index = 0; zone_index < area->zone_count; zone_index++) {
                Zone* zone = area->zones + zone_index;
                if (zone->spawners == NULL) {
                    continue;
                }
                for (int spawner_index = 0; spawner_index < zone->spawner_count; spawner_index++) {
                    free(zone->spawners[spawner_index].enemy_types);
                }
                free(zone->spawners);
            }
            free(area->zones);
            enemy_set_destroy(&area->enemy_set);
        }
        free(region->areas);
    }
    free(map->regions);
    map->spawn_region = NULL;
    map->regions = NULL;
    map->region_count = 0;
}