#ifndef COMMAND_STATE_INCLUDED_0DA981CB_5A87_44AC_BC22_50C05C151846
#define COMMAND_STATE_INCLUDED_0DA981CB_5A87_44AC_BC22_50C05C151846


#include "../gfx_common.hpp"
#include "../clear.hpp"
#include "../ogl/ogl.hpp"


namespace Graphics_api {
namespace Detail {


enum class Buffer_state : uint8_t
{
  clear_target,
  set_vertex_attribute,
  set_geometry,
  set_uniform,
  set_shader,
  draw_call,
};


struct Bind_shader
{
  GLuint shader_id = 0;
};


struct Bind_vertex_format
{
  GLchar    name[32]  = "";
  GLenum    type      = 0;
  GLuint    size      = 0;
  uintptr_t pointer   = 0;
};


struct Bind_mesh
{
  GLuint vbo_id = 0;
  GLuint vbo_entries = 0;
  
  GLuint ibo_id = 0;
};


struct Bind_data1
{
  union
  {
    float     float_data;
    uint32_t  uint_data;
    int32_t   int_data;
  };
};

struct Bind_data2
{
  union
  {
    float     float_data[2];
    uint32_t  uint_data[2];
    int32_t   int_data[2];
  };
};


struct Bind_data3
{
  union
  {
    float     float_data[3];
    uint32_t  uint_data[3];
    int32_t   int_data[3];
  };
};


struct Bind_data4
{
  union
  {
    float     float_data[4];
    uint32_t  uint_data[4];
    int32_t   int_data[4];
  };
};

struct Bind_data9
{
  union
  {
    float     float_data[9];
    uint32_t  uint_data[9];
    int32_t   int_data[9];
  };
};

struct Bind_data16
{
  union
  {
    float     float_data[16];
    uint32_t  uint_data[16];
    int32_t   int_data[16];
  };
};

} // ns
} // ns


#endif // inc guard