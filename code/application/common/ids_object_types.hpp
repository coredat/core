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
  
  dev_dynamic_cube,
  dev_view_cube,
  
  dev_static_ground,
  
  dev_actor,
  dev_gun,
  dev_npc_actor,

}; // enum
} // ns


#endif // inc guard