#ifndef RENDERER_TYPE_INCLUDED_4C34C957_EA88_4123_9B6D_8FB144365AEC
#define RENDERER_TYPE_INCLUDED_4C34C957_EA88_4123_9B6D_8FB144365AEC


#include <stdint.h>


namespace Core {


enum class Renderer_type : uint32_t
{
  unknown = 0,
  material,
  text,
};


} // ns


#endif // inc guard