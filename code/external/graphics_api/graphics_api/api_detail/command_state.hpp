#ifndef COMMAND_STATE_INCLUDED_0DA981CB_5A87_44AC_BC22_50C05C151846
#define COMMAND_STATE_INCLUDED_0DA981CB_5A87_44AC_BC22_50C05C151846


#include "../gfx_common.hpp"
#include "../clear.hpp"


namespace Graphics_api {
namespace Detail {


enum class Buffer_state : uint8_t
{
  clear_target,
  set_vertex_format,
  set_geometry,
  set_uniform,
  set_shader,
  draw_call,
};


} // ns
} // ns


#endif // inc guard