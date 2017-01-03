#ifdef OP_BUFFER_API_TEST
#include "test_api.hpp"
#include "../../common/command_structures.hpp"
#include "../../data/context_data.hpp"
#include <stdio.h>


namespace op {
namespace test_api {


/*
  Lazy generator to create functions for testing.
*/
#define TEST_API_FUNCTION(name)                                     \
static void                                                         \
name (context_data *context, void *data) {                          \
  auto *cmd = reinterpret_cast<op::command::cmd_unknown*>(data);    \
  context->log("- %s ID: %d, Size: %d", #name, cmd->id, cmd->size); \
}                                                                   \

// ** Invalid command ** //

TEST_API_FUNCTION(nothing)

// ** Context ** //

TEST_API_FUNCTION(context_flush)

// ** Device ** //

TEST_API_FUNCTION(device_create)
TEST_API_FUNCTION(device_reset)
TEST_API_FUNCTION(device_destroy)

// ** Vertex format ** //

TEST_API_FUNCTION(vertex_format_create)
TEST_API_FUNCTION(vertex_format_destroy)
TEST_API_FUNCTION(vertex_format_bind)

// ** Shaders ** //

TEST_API_FUNCTION(shader_create)
TEST_API_FUNCTION(shader_destroy)
TEST_API_FUNCTION(shader_bind)

// ** Shader Data ** //

TEST_API_FUNCTION(shader_data_create)
TEST_API_FUNCTION(shader_data_destroy)
TEST_API_FUNCTION(shader_data_bind)

// ** Textures ** //

TEST_API_FUNCTION(texture_create)
TEST_API_FUNCTION(texture_update)
TEST_API_FUNCTION(texture_destroy)
TEST_API_FUNCTION(texture_read)
TEST_API_FUNCTION(texture_bind)

// ** Texture Filtering ** //

TEST_API_FUNCTION(texture_filter_create)
TEST_API_FUNCTION(texture_filter_update)
TEST_API_FUNCTION(texture_filter_destroy)
TEST_API_FUNCTION(texture_filter_bind)

// ** Bufffer ** //

TEST_API_FUNCTION(buffer_create)
TEST_API_FUNCTION(buffer_update)
TEST_API_FUNCTION(buffer_destroy)
TEST_API_FUNCTION(buffer_read)
TEST_API_FUNCTION(buffer_bind)

// ** Geometry ** //

TEST_API_FUNCTION(geometry_create)
TEST_API_FUNCTION(geometry_update)
TEST_API_FUNCTION(geometry_destroy)
TEST_API_FUNCTION(geometry_bind)

// ** Target ** //

TEST_API_FUNCTION(target_create)
TEST_API_FUNCTION(target_update)
TEST_API_FUNCTION(target_clear)
TEST_API_FUNCTION(target_destroy)
TEST_API_FUNCTION(target_clear)
TEST_API_FUNCTION(target_bind)

// ** Rasterizer ** //

TEST_API_FUNCTION(rasterizer_create)
TEST_API_FUNCTION(rasterizer_update)
TEST_API_FUNCTION(rasterizer_destroy)
TEST_API_FUNCTION(rasterizer_bind)

// ** Render ** //

TEST_API_FUNCTION(render_generic)
TEST_API_FUNCTION(render_subset)

// ** Misc ** //

TEST_API_FUNCTION(viewport_bind)


void
initialize(
  op::command::cmd_api_function *api_array,
  size_t api_array_count
)
{
  // Param check
  assert(api_array);
  assert(api_array_count);

  // Connect up actions.

  printf("Initialize Test API\n");

  /*
    Connect the bindings.
  */

  // ** Invalid command ** //

  api_array[op::command::NULL_CMD] = nothing;

  // ** Context ** //

  api_array[op::command::CONTEXT_FLUSH_CACHE] = context_flush;

  // ** Device ** //

  api_array[op::command::DEVICE_INIT]    = device_create;
  api_array[op::command::DEVICE_RESET]   = device_reset;
  api_array[op::command::DEVICE_DESTROY] = device_destroy;

  // ** Vertex format ** //

  api_array[op::command::VERTEX_FORMAT_CREATE]  = vertex_format_create;
  api_array[op::command::VERTEX_FORMAT_DESTROY] = vertex_format_destroy;
  api_array[op::command::VERTEX_FORMAT_BIND]    = vertex_format_bind;

  // ** Shaders ** //

  api_array[op::command::SHADER_CREATE]   = shader_create;
  api_array[op::command::SHADER_DESTROY]  = shader_destroy;
  api_array[op::command::SHADER_BIND]     = shader_bind;

  // ** Shader Data ** //

  api_array[op::command::SHADER_DATA_CREATE]  = shader_data_create;
  api_array[op::command::SHADER_DATA_DESTROY] = shader_data_destroy;
  api_array[op::command::SHADER_DATA_BIND]    = shader_data_bind;

  // ** Textures ** //

  api_array[op::command::TEXTURE_CREATE]  = texture_create;
  api_array[op::command::TEXTURE_UPDATE]  = texture_update;
  api_array[op::command::TEXTURE_DESTROY] = texture_destroy;
  api_array[op::command::TEXTURE_READ]    = texture_read;
  api_array[op::command::TEXTURE_BIND]    = texture_bind;

  // ** Texture Filtering ** //

  api_array[op::command::TEXTURE_FILTER_CREATE]  = texture_filter_create;
  api_array[op::command::TEXTURE_FILTER_UPDATE]  = texture_filter_update;
  api_array[op::command::TEXTURE_FILTER_DESTROY] = texture_filter_destroy;
  api_array[op::command::TEXTURE_FILTER_BIND]    = texture_filter_bind;

  // ** Buffer ** //

  api_array[op::command::BUFFER_CREATE]  = buffer_create;
  api_array[op::command::BUFFER_UPDATE]  = buffer_update;
  api_array[op::command::BUFFER_DESTROY] = buffer_destroy;
  api_array[op::command::BUFFER_READ]    = buffer_read;
  api_array[op::command::BUFFER_BIND]    = buffer_bind;

  // ** Geometry ** //

  api_array[op::command::GEOMETRY_CREATE]  = geometry_create;
  api_array[op::command::GEOMETRY_UPDATE]  = geometry_update;
  api_array[op::command::GEOMETRY_DESTROY] = geometry_destroy;
  api_array[op::command::GEOMETRY_BIND]    = geometry_bind;

  // ** Target ** //

  api_array[op::command::TARGET_CREATE]  = target_create;
  api_array[op::command::TARGET_UPDATE]  = target_update;
  api_array[op::command::TARGET_DESTROY] = target_destroy;
  api_array[op::command::TARGET_CLEAR]   = target_clear;
  api_array[op::command::TARGET_BIND]    = target_bind;

  // ** Rasterizer ** //

  api_array[op::command::RASTERIZER_CREATE]  = rasterizer_create;
  api_array[op::command::RASTERIZER_UPDATE]  = rasterizer_update;
  api_array[op::command::RASTERIZER_DESTROY] = rasterizer_destroy;
  api_array[op::command::RASTERIZER_BIND]    = rasterizer_bind;

  // ** Render ** //

  api_array[op::command::RENDER_GENERIC] = render_generic;
  api_array[op::command::RENDER_SUBSET]  = render_subset;

  // ** Misc ** //

  api_array[op::command::VIEWPORT_BIND] = viewport_bind;

}


} // ns
} // ns


#endif // OP_BUFFER_API_TEST
