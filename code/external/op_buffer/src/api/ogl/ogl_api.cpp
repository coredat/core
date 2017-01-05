#ifdef OP_BUFFER_API_OGL

#include "ogl_api.hpp"
#include "details/ogl_common.hpp"
#include "details/op_common.hpp"

#include "index.hpp"
#include "render.hpp"
#include "geometry.hpp"
#include "debug.hpp"
#include "device.hpp"
#include "target.hpp"
#include "shader.hpp"
#include "shader_data.hpp"
#include "vertex_format.hpp"
#include "texture.hpp"
#include "rasterizer.hpp"
#include "texture_filter.hpp"
#include "misc.hpp"
#include "descs/descs.hpp"


namespace op {
namespace ogl {


static void
nothing(context_data *context, void *data)
{
  context->log("Nothing");
}


void
initialize(
  op::command::cmd_api_function *api_array,
  size_t api_array_count)
{
  // Param check
  assert(api_array);
  assert(api_array_count);

  // Connect up actions.
  api_array[op::command::NULL_CMD]        = nothing;
  api_array[op::command::SHADER_CREATE]   = shader_create;
  api_array[op::command::SHADER_BIND]     = shader_bind;


  // -- Device -- //

  api_array[op::command::DEVICE_INIT]    = device_init;
  api_array[op::command::DEVICE_RESET]   = device_reset;
  api_array[op::command::DEVICE_DESTROY] = device_destroy;

  // -- Vertex Format -- //

  api_array[op::command::VERTEX_FORMAT_CREATE]  = vertex_fmt_create;
  api_array[op::command::VERTEX_FORMAT_DESTROY] = vertex_fmt_destroy;
  api_array[op::command::VERTEX_FORMAT_BIND]    = vertex_fmt_bind;

  // -- Geometry -- //

  api_array[op::command::GEOMETRY_CREATE]  = geometry_create;
  api_array[op::command::GEOMETRY_UPDATE]  = geometry_update;
  api_array[op::command::GEOMETRY_DESTROY] = geometry_destroy;
  api_array[op::command::GEOMETRY_BIND]    = geometry_bind;

  // -- Index -- //

  api_array[op::command::INDEX_CREATE]  = index_create;
  api_array[op::command::INDEX_UPDATE]  = index_update;
  api_array[op::command::INDEX_DESTROY] = index_destroy;
  api_array[op::command::INDEX_BIND]    = index_bind;

  // -- Shader -- //

  api_array[op::command::SHADER_CREATE]  = shader_create;
  api_array[op::command::SHADER_DESTROY] = shader_destroy;
  api_array[op::command::SHADER_BIND]    = shader_bind;

  // -- Shader Data -- //

  api_array[op::command::SHADER_DATA_CREATE]  = shader_data_create;
  api_array[op::command::SHADER_DATA_DESTROY] = shader_data_destroy;
  api_array[op::command::SHADER_DATA_BIND]    = shader_data_bind;

  // -- Texture -- //

  api_array[op::command::TEXTURE_CREATE]  = texture_create;
  api_array[op::command::TEXTURE_UPDATE]  = texture_update;
  api_array[op::command::TEXTURE_DESTROY] = texture_destroy;
  // TEXTURE_READ,
  api_array[op::command::TEXTURE_BIND]    = texture_bind;

  // -- Texture Filtering -- //

  api_array[op::command::TEXTURE_FILTER_CREATE]  = texture_filter_create;
  api_array[op::command::TEXTURE_FILTER_UPDATE]  = texture_filter_update;
  api_array[op::command::TEXTURE_FILTER_DESTROY] = texture_filter_destroy;
  api_array[op::command::TEXTURE_FILTER_BIND]    = texture_filter_bind;

  // -- Target -- //

  api_array[op::command::TARGET_CREATE]  = target_create;
  api_array[op::command::TARGET_UPDATE]  = target_update;
  api_array[op::command::TARGET_CLEAR]   = target_clear;
  api_array[op::command::TARGET_DESTROY] = target_destroy;
  api_array[op::command::TARGET_CLEAR]   = target_clear;
  api_array[op::command::TARGET_BIND]    = target_bind;

  // -- Rendering -- //

  api_array[op::command::RENDER_GENERIC] = render_generic;
  api_array[op::command::RENDER_SUBSET]  = render_subset;

  // -- Rasterizer -- //

  api_array[op::command::RASTERIZER_CREATE]  = rasterizer_create;
  api_array[op::command::RASTERIZER_UPDATE]  = rasterizer_update;
  api_array[op::command::RASTERIZER_DESTROY] = rasterizer_destroy;
  api_array[op::command::RASTERIZER_BIND]    = rasterizer_bind;

  // -- Misc -- //

  api_array[op::command::VIEWPORT_BIND] = viewport_set;

  // -- Debug -- //

  api_array[op::command::DEBUG_MARKER_PUSH] = debug_marker_push;
  api_array[op::command::DEBUG_MARKER_POP]  = debug_marker_pop;

}


} // ns
} // ns

#endif // api
