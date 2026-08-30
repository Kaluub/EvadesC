#pragma once

#include <stdint.h>

enum EnemyType {
    ENEMY_TYPE_NONE,
    ENEMY_TYPE_WALL,
    ENEMY_TYPE_NORMAL,
    ENEMY_TYPE_HOMING,
    ENEMY_TYPE_HOMING_SWITCH,
    ENEMY_TYPE_DASHER,
    ENEMY_TYPE_DASHER_SWITCH,
    ENEMY_TYPE_SLOWING,
    ENEMY_TYPE_DRAINING,
    ENEMY_TYPE_GRAVITY,
    ENEMY_TYPE_REPELLING,
    ENEMY_TYPE_TURNING,
    ENEMY_TYPE_SIZING,
    ENEMY_TYPE_SNIPER,
    ENEMY_TYPE_FREEZING,
    ENEMY_TYPE_TELEPORTING,
    ENEMY_TYPE_WAVY,
    ENEMY_TYPE_WAVY_SWITCH,
    ENEMY_TYPE_ZIGZAG,
    ENEMY_TYPE_ZIGZAG_SWITCH,
    ENEMY_TYPE_CONFECTIONER,
    ENEMY_TYPE_CONFECTIONER_SWITCH,
    ENEMY_TYPE_ZONING,
    ENEMY_TYPE_ZONING_SWITCH,
    ENEMY_TYPE_SPIRAL,
    ENEMY_TYPE_SPIRAL_SWITCH,
    ENEMY_TYPE_OSCILLATING,
    ENEMY_TYPE_OSCILLATING_SWITCH,
    ENEMY_TYPE_SWITCH,
    ENEMY_TYPE_DORITO,
    ENEMY_TYPE_DORITO_SWITCH,
    ENEMY_TYPE_PENNY,
    ENEMY_TYPE_PENNY_SWITCH,
    ENEMY_TYPE_INFINITY,
    ENEMY_TYPE_INFINITY_SWITCH,
    ENEMY_TYPE_LIQUID,
    ENEMY_TYPE_ICICLE,
    ENEMY_TYPE_SLIPPERY,
    ENEMY_TYPE_ICE_SNIPER,
    ENEMY_TYPE_DISABLING,
    ENEMY_TYPE_EXPERIENCE_DRAIN,
    ENEMY_TYPE_ENLARGING,
    ENEMY_TYPE_SPEED_SNIPER,
    ENEMY_TYPE_REGEN_SNIPER,
    ENEMY_TYPE_RADIATING_BULLETS,
    ENEMY_TYPE_IMMUNE,
    ENEMY_TYPE_PUMPKIN,
    ENEMY_TYPE_FAKE_PUMPKIN,
    ENEMY_TYPE_TREE,
    ENEMY_TYPE_FROST_GIANT,
    ENEMY_TYPE_SNOWMAN,
    ENEMY_TYPE_CORROSIVE,
    ENEMY_TYPE_TOXIC,
    ENEMY_TYPE_CORROSIVE_SNIPER,
    ENEMY_TYPE_POISON_SNIPER,
    ENEMY_TYPE_MAGNETIC_REDUCTION,
    ENEMY_TYPE_MAGNETIC_NULLIFICATION,
    ENEMY_TYPE_POSITIVE_MAGNETIC_SNIPER,
    ENEMY_TYPE_NEGATIVE_MAGNETIC_SNIPER,
    ENEMY_TYPE_RESIDUE,
    ENEMY_TYPE_FIRE_TRAIL,
    ENEMY_TYPE_ICE_GHOST,
    ENEMY_TYPE_POISON_GHOST,
    ENEMY_TYPE_POSITIVE_MAGNETIC_GHOST,
    ENEMY_TYPE_NEGATIVE_MAGNETIC_GHOST,
    ENEMY_TYPE_WIND_GHOST,
    ENEMY_TYPE_LUNGING,
    ENEMY_TYPE_LAVA,
    ENEMY_TYPE_GRAVITY_GHOST,
    ENEMY_TYPE_REPELLING_GHOST,
    ENEMY_TYPE_STAR,
    ENEMY_TYPE_GRASS,
    ENEMY_TYPE_SEEDLING,
    ENEMY_TYPE_FLOWER,
    ENEMY_TYPE_DISABLING_GHOST,
    ENEMY_TYPE_GLOWY,
    ENEMY_TYPE_FIREFLY,
    ENEMY_TYPE_MIST,
    ENEMY_TYPE_PHANTOM,
    ENEMY_TYPE_CYBOT,
    ENEMY_TYPE_EABOT,
    ENEMY_TYPE_WABOT,
    ENEMY_TYPE_FIBOT,
    ENEMY_TYPE_AIBOT,
    ENEMY_TYPE_WIND_SNIPER,
    ENEMY_TYPE_SAND,
    ENEMY_TYPE_SANDROCK,
    ENEMY_TYPE_QUICKSAND,
    ENEMY_TYPE_CRUMBLING,
    ENEMY_TYPE_RADAR,
    ENEMY_TYPE_BARRIER,
    ENEMY_TYPE_SPEED_GHOST,
    ENEMY_TYPE_REGEN_GHOST,
    ENEMY_TYPE_CACTUS,
    ENEMY_TYPE_CYCLING,
    ENEMY_TYPE_ICBOT,
    ENEMY_TYPE_ELBOT,
    ENEMY_TYPE_PLBOT,
    ENEMY_TYPE_MEBOT,
    ENEMY_TYPE_LIBOT,
    ENEMY_TYPE_DABOT,
    ENEMY_TYPE_SPARKING,
    ENEMY_TYPE_THUNDERBOLT,
    ENEMY_TYPE_STATIC,
    ENEMY_TYPE_ELECTRICAL,
    ENEMY_TYPE_POWERED,
    ENEMY_TYPE_PREDICTION_SNIPER,
    ENEMY_TYPE_RING_SNIPER,
    ENEMY_TYPE_CHARGING,
    ENEMY_TYPE_REDUCING,
    ENEMY_TYPE_LEAD_SNIPER,
    ENEMY_TYPE_STALACTITE,
    ENEMY_TYPE_BLOCKING,
    ENEMY_TYPE_FORCE_SNIPER_A,
    ENEMY_TYPE_FORCE_SNIPER_B,
    ENEMY_TYPE_WACKY_WALL,
    ENEMY_TYPE_FLAMING,
    ENEMY_TYPE_STUMBLING,
    ENEMY_TYPE_DISARMING,
    ENEMY_TYPE_LURCHING,
    ENEMY_TYPE_INFECTIOUS,
    ENEMY_TYPE_MUTATING,
    ENEMY_TYPE_VENGEFUL_SOUL,
    ENEMY_TYPE_LOST_SOUL,
    ENEMY_TYPE_BLIND,
    ENEMY_TYPE_NINJA_STAR_SNIPER,
    ENEMY_TYPE_SUMMONER,
    ENEMY_TYPE_SLASHER,
    ENEMY_TYPE_LOTUS_FLOWER,
    ENEMY_TYPE_VOID_CRAWLER,
    ENEMY_TYPE_VOID_SWARM,
    ENEMY_TYPE_VOID_SNIPER,
    ENEMY_TYPE_DRIPPING,
    ENEMY_TYPE_VOID_DRAIN,
    ENEMY_TYPE_WITHERING,
    ENEMY_TYPE_SUPERSTAR,
    ENEMY_TYPE_ENFORCING,
    ENEMY_TYPE_TRISNIPER,
    ENEMY_TYPE_MULTISNIPER,
    ENEMY_TYPE_CRYSTAL_GIANT,
    ENEMY_TYPE_CRYSTAL_GHOST,
    ENEMY_TYPE_GROWING,
    ENEMY_TYPE_CRYSTAL_WALL,
};

static const uint32_t enemy_colors[] = {
    0x0, // none
    0x222222ff, // wall_enemy
    0x939393ff, // normal_enemy
    0x966e14ff, // homing_enemy
    0x694d0eff, // homing_switch_enemy
    0x003c66ff, // dasher_enemy
    0x00243dff, // dasher_switch_enemy
    0xff0000ff, // slowing_enemy
    0x0000ffff, // draining_enemy
    0x78148cff, // gravity_enemy
    0x7b9db2ff, // repelling_enemy
    0x336600ff, // turning_enemy
    0xf27743ff, // sizing_enemy
    0xa05353ff, // sniper_enemy
    0x64c1b9ff, // freezing_enemy
    0xecc4efff, // teleporting_enemy
    0xdd2606ff, // wavy_enemy
    0xfa5336ff, // wavy_switch_enemy
    0xb371f2ff, // zigzag_enemy
    0xe0c6f9ff, // zigzag_switch_enemy
    0x8771f2ff, // confectioner_enemy
    0xcfc6f9ff, // confectioner_switch_enemy
    0xa03811ff, // zoning_enemy
    0xb35f40ff, // zoning_switch_enemy
    0xe8b500ff, // spiral_enemy
    0xf5e199ff, // spiral_switch_enemy
    0x869e0fff, // oscillating_enemy
    0xb6c46fff, // oscillating_switch_enemy
    0x565656ff, // switch_enemy
    0x05dad1ff, // dorito_enemy
    0x9bf0ecff, // dorito_switch_enemy
    0xc38b32ff, // penny_enemy
    0xd9b67fff, // penny_switch_enemy
    0xff69c5ff, // infinity_enemy
    0xffb4e2ff, // infinity_switch_enemy
    0x6789efff, // liquid_enemy
    0xadf8ffff, // icicle_enemy
    0x1aacbfff, // slippery_enemy
    0x8300ffff, // ice_sniper_enemy
    0xa87c86ff, // disabling_enemy
    0xb19cd9ff, // experience_drain_enemy
    0x4d0163ff, // enlarging_enemy
    0xff9000ff, // speed_sniper_enemy
    0x00cc8eff, // regen_sniper_enemy
    0xd3134fff, // radiating_bullets_enemy
    0x000000ff, // immune_enemy
    0xe26110ff, // pumpkin_enemy
    0xe26110ff, // fake_pumpkin_enemy
    0x4e2700ff, // tree_enemy
    0x7e7cd6ff, // frost_giant_enemy
    0xffffffff, // snowman_enemy
    0x00eb00ff, // corrosive_enemy
    0x00c700ff, // toxic_enemy
    0x61ff61ff, // corrosive_sniper_enemy
    0x8c01b7ff, // poison_sniper_enemy
    0xbd67d2ff, // magnetic_reduction_enemy
    0x642374ff, // magnetic_nullification_enemy
    0xff3852ff, // positive_magnetic_sniper_enemy
    0xa496ffff, // negative_magnetic_sniper_enemy
    0x675327ff, // residue_enemy
    0xcf5504ff, // fire_trail_enemy
    0xbe89ff66, // ice_ghost_enemy
    0x59017466, // poison_ghost_enemy
    0xe3001e66, // positive_magnetic_ghost_enemy
    0x6f59ff66, // negative_magnetic_ghost_enemy
    0x9de3c666, // wind_ghost_enemy
    0xc88250ff, // lunging_enemy
    0xf78306ff, // lava_enemy
    0x78148c66, // gravity_ghost_enemy
    0x7b9db266, // repelling_ghost_enemy
    0xfaf46eff, // star_enemy
    0x75eb26ff, // grass_enemy
    0x259c55ff, // seedling_enemy
    0xe8e584ff, // flower_enemy
    0xffbfce33, // disabling_ghost_enemy
    0xede658ff, // glowy_enemy
    0xf0841fff, // firefly_enemy
    0xb686dbff, // mist_enemy
    0x86d7dbff, // phantom_enemy
    0x926be3ff, // cybot_enemy
    0xb07331ff, // eabot_enemy
    0x319bb0ff, // wabot_enemy
    0xe88409ff, // fibot_enemy
    0x00b585ff, // aibot_enemy
    0x9de3c6ff, // wind_sniper_enemy
    0xd5ae7fff, // sand_enemy
    0xa57a6dff, // sandrock_enemy
    0x6c541eff, // quicksand_enemy
    0xbd9476ff, // crumbling_enemy
    0xc90000ff, // radar_enemy
    0x29ffc6ff, // barrier_enemy
    0xfca33066, // speed_ghost_enemy
    0x32e3ae66, // regen_ghost_enemy
    0x5b8e28ff, // cactus_enemy
    0x91bbffff, // cycling_enemy
    0x1bc8e3ff, // icbot_enemy
    0xdaff1fff, // elbot_enemy
    0x18ed3fff, // plbot_enemy
    0xb55b31ff, // mebot_enemy
    0xfff9bdff, // libot_enemy
    0x3d006eff, // dabot_enemy
    0xffbe6eff, // sparking_enemy
    0xf4ff8cff, // thunderbolt_enemy
    0xf5a462ff, // static_enemy
    0x2fded7ff, // electrical_enemy
    0xc2c2c2ff, // powered_enemy
    0xd14f84ff, // prediction_sniper_enemy
    0xb5deebff, // ring_sniper_enemy
    0x374037ff, // charging_enemy
    0x2d3237ff, // reducing_enemy
    0x788898ff, // lead_sniper_enemy
    0x302519ff, // stalactite_enemy
    0xbf5213ff, // blocking_enemy
    0x0a5557ff, // force_sniper_a_enemy
    0x914d83ff, // force_sniper_b_enemy
    0x332233ff, // wacky_wall_enemy
    0xaa2f2fff, // flaming_enemy
    0x7d487fff, // stumbling_enemy
    0xa377a3ff, // disarming_enemy
    0x5d4d5dff, // lurching_enemy
    0xeb00ebff, // infectious_enemy
    0x211513ff, // mutating_enemy
    0x96b1b3ff, // vengeful_soul_enemy
    0xbed0d1ff, // lost_soul_enemy
    0x96c6ecff, // blind_enemy
    0xdededeff, // ninja_star_sniper_enemy
    0x91bbffff, // summoner_enemy
    0x363636ff, // slasher_enemy
    0xdededeff, // lotus_flower_enemy
    0x1c0a2dff, // void_crawler_enemy
    0x393042ff, // void_swarm_enemy
    0x40144bff, // void_sniper_enemy
    0x100812ff, // dripping_enemy
    0x261235ff, // void_drain_enemy
    0x752656ff, // withering_enemy
    0xffffffff, // superstar_enemy
    0x590016ff, // enforcing_enemy
    0x63464bff, // trisniper_enemy
    0x8a8769ff, // multisniper_enemy
    0xc0297aff, // crystal_giant_enemy
    0xc0297a66, // crystal_ghost_enemy
    0xbe4db2ff, // growing_enemy
    0x5a0078ff, // crystal_wall_enemy
};