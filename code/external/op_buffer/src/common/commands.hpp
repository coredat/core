#ifndef COMMANDS_INCLUDED_2DB97206_E980_4114_8CE3_4CA238EF7807
#define COMMANDS_INCLUDED_2DB97206_E980_4114_8CE3_4CA238EF7807


#include <stdint.h>


namespace op {
namespace command {



enum ENUM : uint8_t
{
  // -- Invalid command -- //

  NULL_CMD,

  // -- Context -- //

  CONTEXT_FLUSH_CACHE,

  // -- Device -- //

  DEVICE_INIT,
  DEVICE_RESET,
  DEVICE_DESTROY,

  // -- Vertex format -- //

  VERTEX_FORMAT_CREATE,
  VERTEX_FORMAT_DESTROY,
  VERTEX_FORMAT_BIND,

  // -- Shaders -- //

  SHADER_CREATE,
  SHADER_DESTROY,
  SHADER_BIND,

  // -- Compute Shaders -- //

  COMPUTE_SHADER_CREATE,
  COMPUTE_SHADER_DESTROY,
  COMPUTE_SHADER_BIND,

  // -- Shader Data -- //

  SHADER_DATA_CREATE,
  SHADER_DATA_DESTROY,
  SHADER_DATA_BIND,

  // -- Textures -- //

  TEXTURE_CREATE,
  TEXTURE_UPDATE,
  TEXTURE_DESTROY,
  TEXTURE_READ,
  TEXTURE_BIND,

  // -- Buffer -- //

  BUFFER_CREATE,
  BUFFER_UPDATE,
  BUFFER_DESTROY,
  BUFFER_READ,
  BUFFER_BIND,

  // -- Geometry -- //

  GEOMETRY_CREATE,
  GEOMETRY_UPDATE,
  GEOMETRY_DESTROY,
  GEOMETRY_BIND,

  // -- Index -- //

  INDEX_CREATE,
  INDEX_UPDATE,
  INDEX_DESTROY,
  INDEX_BIND,

  // -- Target -- //

  TARGET_CREATE,
  TARGET_UPDATE,
  TARGET_CLEAR,
  TARGET_DESTROY,
  TARGET_BIND,

  // -- Rasterizer -- //

  RASTERIZER_CREATE,
  RASTERIZER_UPDATE,
  RASTERIZER_DESTROY,
  RASTERIZER_BIND,

  // -- Blend -- //

  BLEND_CREATE,
  BLEND_UPDATE,
  BLEND_DESTROY,
  BLEND_BIND,

  // -- Render -- //

  RENDER_GENERIC,
  RENDER_SUBSET,

  // -- Misc -- //

  VIEWPORT_BIND,

  // -- Debug -- //

  DEBUG_MARKER_PUSH,
  DEBUG_MARKER_POP,

  // -- Experimental -- //

  BENCHMARK_CREATE,
  BENCHMARK_START,
  BENCHMARK_END,

  // -- Number of Commands -- //

  COUNT

  // -- Nothing Under COUNT -- //
};


} // ns
} // ns


#endif // inc guard
