#ifndef IDS_GAME_EVENTS_INCLUDED_CE5A6074_04D4_4BEC_849F_D7D6A90F37EA
#define IDS_GAME_EVENTS_INCLUDED_CE5A6074_04D4_4BEC_849F_D7D6A90F37EA


#include <stdint.h>


namespace Game_event_id {
enum ENUM : uint32_t {

  got_shot,
  
  gun_fire,
  
  actor_died, // Signal from the actor to its controller.

  player_spawned,
  player_died,
  player_walk,
  
  guards_sounded_alarm,
  
  game_state_initial_load,
  game_state_start_menu,
  game_state_playing,
  game_state_game_over,

}; // enum
} // ns


#endif // inc gaurd