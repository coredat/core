#ifndef IDS_GAME_EVENTS_INCLUDED_CE5A6074_04D4_4BEC_849F_D7D6A90F37EA
#define IDS_GAME_EVENTS_INCLUDED_CE5A6074_04D4_4BEC_849F_D7D6A90F37EA


#include <core/ids_core_events.hpp>
#include <stdint.h>


namespace Game_event_id {
enum ENUM : uint32_t {

  null_event   = Core::Event_id::end_of_core_events,

  got_shot,
  
  gun_fire,
  
  actor_died, // Signal from the actor to its controller.

  player_spawned,
  player_died,
  player_walk,

}; // enum
} // ns


#endif // inc gaurd