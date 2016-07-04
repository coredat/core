#ifndef CAMERA_PROPERTIES_INCLUDED_7177B635_304E_4359_BC85_8F59BE947FCB
#define CAMERA_PROPERTIES_INCLUDED_7177B635_304E_4359_BC85_8F59BE947FCB


#include <stdint.h>
#include <utilities/bits.hpp>


namespace Core {


enum class Camera_type : uint32_t
{
  perspective,
  orthographic,
};


namespace Camera_clear {
enum ENUM : uint32_t
{
  color = BIT(0),
  depth = BIT(1),
};
} // ns


} // ns


#endif // inc guard