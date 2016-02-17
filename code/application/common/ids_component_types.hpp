#ifndef IDS_COMPONENT_TYPES_INCLUDED_812B8BE4_5DF7_4585_B086_3C71CE3BC2BE
#define IDS_COMPONENT_TYPES_INCLUDED_812B8BE4_5DF7_4585_B086_3C71CE3BC2BE


#include <stdint.h>


namespace Component_type {
enum ENUM : uint32_t {

  invalid,    // zero 

  actor,
  gun,
  
  local_player_controller,
  network_player_controller,
  npc_actor_controller,

}; // enum
} // ns


#endif // inc guard