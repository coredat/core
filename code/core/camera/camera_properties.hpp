#ifndef CAMERA_PROPERTIES_INCLUDED_7177B635_304E_4359_BC85_8F59BE947FCB
#define CAMERA_PROPERTIES_INCLUDED_7177B635_304E_4359_BC85_8F59BE947FCB


#include <stdint.h>


namespace Core {


enum class Camera_type : uint32_t
{
  perspective,
};


namespace Camera_clear {
enum ENUM : uint32_t
{
  color = 1 << 0,
  depth = 1 << 1,
};
} // ns


} // ns


#endif // inc guard