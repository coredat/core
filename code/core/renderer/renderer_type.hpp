#ifndef RENDERER_TYPE_INCLUDED_4C34C957_EA88_4123_9B6D_8FB144365AEC
#define RENDERER_TYPE_INCLUDED_4C34C957_EA88_4123_9B6D_8FB144365AEC


#include <stdint.h>
#include <common/data_types.hpp>


namespace Core {
namespace Renderer_type {

enum ENUM : uint32_t
{
  unknown   = 0,
  material  = Common::Data_type::renderer_mesh,
  text      = Common::Data_type::renderer_text,
};

} // ns
} // ns


#endif // inc guard