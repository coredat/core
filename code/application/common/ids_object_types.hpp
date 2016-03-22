#ifndef IDS_OBJECT_TYPES_INCLUDED_C2D671C0_55D7_4C53_BB36_DCDDA9922DB0
#define IDS_OBJECT_TYPES_INCLUDED_C2D671C0_55D7_4C53_BB36_DCDDA9922DB0


#include <stdint.h>


/*!
  Type ids are the type id's used in Core::Entity
*/
namespace Object_type {
enum ENUM : uint32_t {

  null_type = 0,
  
  dev_generic,
  
  game_camera,
  explosion,
  bullet,
  player,
  npc,
  level,
  multiplier,
  
  game_state,

}; // enum
} // ns


#endif // inc guard