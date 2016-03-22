#ifndef IDS_COMPONENT_TYPES_INCLUDED_812B8BE4_5DF7_4585_B086_3C71CE3BC2BE
#define IDS_COMPONENT_TYPES_INCLUDED_812B8BE4_5DF7_4585_B086_3C71CE3BC2BE


#include <stdint.h>


namespace Component_type {
enum ENUM : uint32_t {

  invalid, // zero 

  game_controller,

  actor_controller,
  camera_controller,
  bullet_controller,
  enemy_controller,
  multiplier_controller,
  
  enemy_spawner_model,
  player_stats_model,
  gun_model,
  move_model,
  explosion_model,

}; // enum
} // ns


#endif // inc guard