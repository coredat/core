#ifndef OBJECT_TAGS_INCLUDED_D2B5A0EB_418E_4A1A_A687_039FB6A0D653
#define OBJECT_TAGS_INCLUDED_D2B5A0EB_418E_4A1A_A687_039FB6A0D653


#include <stdint.h>


/*!
  Tags are used to attach to entities, we can use them for identification.
  Tags must be power of two!
*/
namespace Tag {
enum ENUM : uint32_t {

  actor           = 1 << 0,
  npc             = 1 << 1,
  game_state      = 1 << 2,
  player          = 1 << 3,
  network_player  = 1 << 4,
  projectile      = 1 << 5,
  pickup          = 1 << 6,

}; // enum
} // ns


#endif // inc gaurd