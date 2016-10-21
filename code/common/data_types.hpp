#ifndef DATA_TYPES_INCLUDED_1A67BF1D_E0A2_4CE8_BD40_43E6E5C4F07E
#define DATA_TYPES_INCLUDED_1A67BF1D_E0A2_4CE8_BD40_43E6E5C4F07E


#include <stdint.h>


namespace Common {
namespace Data_type {


enum ENUM : uint32_t
{
  transform       = 1 << 0,
  rigidbody       = 1 << 1,
  trigger         = 1 << 2,
  camera          = 1 << 3,
  renderer_mesh   = 1 << 4,
  renderer_text   = 1 << 5,
}; // enum


/*
  Sugar method to check if a data type is present or not.
*/
bool
has_data_type(const uint32_t data_types, const uint32_t type);


/*
  Sugar method to check if any physics data is present.
*/
bool
is_collidable(const uint32_t data_types);


/*
  Sugar method to check if any renderers are attached.
*/
bool
is_renderable(const uint32_t data_types);


/*
  Sugar method to get the active renderer
  This works on the assumption that there can only be 1
  renderer attached.
*/
uint32_t
get_renderer_type(const uint32_t data_types);


} // ns
} // ns


#endif // inc guard