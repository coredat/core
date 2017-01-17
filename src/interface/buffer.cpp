#include "buffer_impl.hpp"
#include "context_impl.hpp"
#include "common_impl.hpp"
#include "resource_id_impl.hpp"

#include "../../include/op/buffer.hpp"
#include "../../include/op/context.hpp"
#include "../common/common.hpp"
#include "../common/commands.hpp"
#include "../common/command_structures.hpp"

#include "../api/unsupported/unsupported.hpp"

#include "../api/ogl/ogl_api.hpp"
#include "../api/test/test_api.hpp"


namespace {

op::command::cmd_api_function api_functions[op::command::COUNT] {0};

} // anon ns


opBuffer*
opBufferCreate(const size_t initial_size)
{
  opBuffer *buffer = new opBuffer;

  buffer->data.alloc_cb   = getCallbackAlloc();
  buffer->data.realloc_cb = getCallbackRealloc();
  buffer->data.destroy_cb = getCallbackFree();
  buffer->data.user_data  = getCallbackUserData();
  buffer->data.initial_reserve(initial_size);

  op::unsupported::initialize(api_functions, op::command::COUNT);
  API::initialize(api_functions, op::command::COUNT);

  return buffer;
}


void
opBufferExec(opContext *ctx, opBuffer *buf)
{
  // -- Param Check -- //
  assert(ctx);

  op::context_data *ctx_data = &ctx->data;
  assert(ctx_data);

  while(buf->data.has_data())
  {
    op::command::cmd_unknown *uk_cmd = reinterpret_cast<op::command::cmd_unknown*>(buf->data.read_data());

    if(uk_cmd->id)
    {
      api_functions[uk_cmd->id](ctx_data, uk_cmd);
    }
  }

  buf->data.reset();
}


void
opBufferFlush(opBuffer *buf)
{
  buf->data.reset();
}


// -- Device -- //


opID
opBufferDeviceCreate(opContext *ctx, opBuffer *buf)
{
  constexpr op::command::cmd_device_init cmd {};

  buf->data.write_data((void*)&cmd, sizeof(cmd));

  printf("Do something about this\n");

  return 0;
}


void
opBufferDeviceReset(opBuffer *buf)
{
  constexpr op::command::cmd_device_reset cmd {};

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


// -- Vertex Format -- //


opID
opBufferVertexFormatCreate(opContext *ctx,
                           opBuffer *buf,
                           opVertexDesc desc[],
                           const size_t attr_count)
{
  // -- Param Check -- //
  assert(ctx);
  assert(desc);

  // -- Generate a pending resource -- //
  op::context_data *ctx_data = &ctx->data;

  const opID new_vertex_format = ctx_data->add_vertex_format();

  op::command::cmd_input_create cmd{};
  cmd.vertex_fmt_id = op::instance_id(new_vertex_format);
  cmd.desc          = desc;
  cmd.desc_count    = attr_count;

  buf->data.write_data((void*)&cmd, sizeof(cmd));

  return (new_vertex_format);
}


void
opBufferVertexFormatBind(opBuffer *buf, const opID id)
{
  // Did you try bind a different resource type?
  assert(op::type_id(id) == op::resource_type::VERTEX_FORMAT);

  op::command::cmd_input_bind cmd {};
  cmd.vertex_fmt_id = op::instance_id(id);

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


// -- Shader -- //


opID
opBufferShaderCreate(opContext *ctx,
                     opBuffer *buf,
                     const char *name,
                     const char *vs,
                     const char *gs,
                     const char *ps,
                     opShaderDesc *in_out_desc)
{
  // -- Param Check -- //
  assert(ctx);
  assert(vs);
  assert(ps);

  // -- Generate a pending resource -- //
  op::context_data *ctx_data = &ctx->data;

  const opID new_shader = ctx_data->add_shader();

  if(op::resource_is_valid(new_shader))
  {
    op::command::cmd_shader_create cmd {};
    cmd.vs   = vs;
    cmd.gs   = gs;
    cmd.ps   = ps;
    cmd.name = name;
    cmd.desc = in_out_desc;
    cmd.shader_id = op::instance_id(new_shader);

    printf("Do I not need to add instance ID?\n");

    buf->data.write_data((void*)&cmd, sizeof(cmd));
  }

  // -- Failed To Generate -- //
  return new_shader;
}


void
opBufferShaderBind(opBuffer *buf, const opID id)
{
  // Did you try bind a different resource type?
  assert(op::type_id(id) == op::resource_type::SHADER);

  op::command::cmd_shader_bind cmd {};
  cmd.shader_id = op::instance_id(id);

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


// -- Shader Data -- //


opID
opBufferShaderDataCreate(opContext *ctx,
                         opBuffer *buf,
                         const opID shader_id,
                         const char *name,
                         opShaderDataDesc *in_out_desc)
{
  // -- Param Check -- //
  assert(ctx);
  assert(buf);
  assert(name);

  // -- Generate a pending resource -- //
  op::context_data *ctx_data = &ctx->data;

  const opID new_shader_data = ctx_data->add_shader_data();

  if(op::resource_is_valid(new_shader_data))
  {
    op::command::cmd_shader_data_create cmd {};
    cmd.shader_data_id = op::instance_id(new_shader_data);
    cmd.shader_id      = op::instance_id(shader_id);
    cmd.name           = name;
    cmd.desc           = in_out_desc;

    buf->data.write_data((void*)&cmd, sizeof(cmd));
  }

  return new_shader_data;
}


void
opBufferShaderDataBind(opBuffer *buf,
                       const opID id,
                       void *data)
{
  // Did you try bind a different resource type?
  assert(op::type_id(id) == op::resource_type::SHADER_DATA);

  op::command::cmd_shader_data_bind cmd {};
  cmd.shader_data_id = op::instance_id(id);
  cmd.data           = (uintptr_t)data;

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


void
opBufferShaderDataBind(opBuffer *buf,
                       const opID id,
                       const opID texture_id)
{
  assert(buf);

  // Did you try bind a different resource type?
  assert(op::type_id(id) == op::resource_type::SHADER_DATA);
  assert(op::type_id(texture_id) == op::resource_type::TEXTURE);

  op::command::cmd_shader_data_bind cmd {};
  cmd.shader_data_id = op::instance_id(id);
  cmd.data           = (uintptr_t)op::instance_id(texture_id);

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


// -- Geometry -- //


opID
opBufferGeometryCreate(opContext *ctx,
                       opBuffer *buf,
                       void *data,
                       const size_t bytes_of_data,
                       const size_t element_count,
                       opGeometryDesc *in_out_desc)
{
  // -- Param Check -- //
  assert(ctx);
  assert(buf);
  assert(in_out_desc);

  // -- Generate Resource -- //
  op::context_data *ctx_data = &ctx->data;

  const opID new_geom = ctx_data->add_geometry();

  if(op::resource_is_valid(new_geom))
  {
    op::command::cmd_geometry_create cmd {};
    cmd.desc          = in_out_desc;
    cmd.geometry_id   = op::instance_id(new_geom);
    cmd.sizeof_data   = bytes_of_data;
    cmd.element_count = element_count;
    cmd.data          = data;

    buf->data.write_data((void*)&cmd, sizeof(cmd));
  }

  return new_geom;
}


void
opBufferGeometryBind(opBuffer *buf, const opID id)
{
  // Did you try bind a different resource type?
  assert(op::type_id(id) == op::resource_type::GEOMETRY);

  op::command::cmd_geometry_bind cmd {};
  cmd.geometry_id = op::instance_id(id);

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


// -- Index -- //


opID
opBufferIndexCreate(opContext *ctx,
                    opBuffer *buf,
                    void *data,
                    const size_t bytes_of_data,
                    const size_t element_count,
                    opIndexDesc *in_out_desc)
{
  // -- Param Check -- //
  assert(ctx);
  assert(buf);
  assert(data);
  assert(bytes_of_data);
  assert(element_count);

  // -- Generate Resource -- //
  op::context_data *ctx_data = &ctx->data;

  const opID new_index = ctx_data->add_index();

  if(op::resource_is_valid(new_index))
  {
    op::command::cmd_index_create cmd {};
    cmd.desc = in_out_desc;
    cmd.data = data;
    cmd.data_bytes = bytes_of_data;
    cmd.element_count = element_count;
    cmd.index_id = op::instance_id(new_index);

    buf->data.write_data((void*)&cmd, sizeof(cmd));
  }

  return new_index;
}


void
opBufferIndexBind(opBuffer *buf, const opID id)
{
  // Did you try bind a different resource type?
  assert(op::type_id(id) == op::resource_type::INDEX);

  op::command::cmd_index_bind cmd {};
  cmd.index_id = op::instance_id(id);

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


// -- Texture -- //


opID
opBufferTextureCreate(opContext *ctx,
                      opBuffer *buf,
                      void *data,
                      opTextureDesc *in_out_desc)
{
  // -- Param Check -- //
  assert(ctx);
  assert(buf);
  assert(in_out_desc);

  // -- Generate Resource -- //
  op::context_data *ctx_data = &ctx->data;

  const opID new_texture = ctx_data->add_texture();

  if(op::resource_is_valid(new_texture))
  {
    op::command::cmd_texture_create cmd {};
    cmd.data = data;
    cmd.desc = in_out_desc;
    cmd.texture_id = op::instance_id(new_texture);

    buf->data.write_data((void*)&cmd, sizeof(cmd));
  }

  return new_texture;
}

void
opBufferTextureUpdate(opContext *ctx,
                      opBuffer *buf,
                      const opID id,
                      const size_t offset_x,
                      const size_t width,
                      void *data)
{
  opBufferTextureUpdate(ctx, buf, id, offset_x, 0, 0, width, 0, 0, data);
}


void
opBufferTextureUpdate(opContext *ctx,
                      opBuffer *buf,
                      const opID id,
                      const size_t offset_x,
                      const size_t offset_y,
                      const size_t width,
                      const size_t height,
                      void *data)
{
  opBufferTextureUpdate(ctx, buf, id, offset_x, offset_y, 0, width, height, 0, data);
}


void
opBufferTextureUpdate(opContext *ctx,
                      opBuffer *buf,
                      const opID id,
                      const size_t offset_x,
                      const size_t offset_y,
                      const size_t offset_z,
                      const size_t width,
                      const size_t height,
                      const size_t depth,
                      void *data)
{
  // -- Param Check -- //
  assert(ctx);
  assert(buf);

  // Did you try bind a different resource type?
  assert(op::type_id(id) == op::resource_type::TEXTURE);

  op::command::cmd_texture_update cmd {};
  cmd.texture_id = op::instance_id(id);
  cmd.offset_x = offset_x;
  cmd.offset_y = offset_y;
  cmd.offset_z = offset_z;
  cmd.width = width;
  cmd.height = height;
  cmd.depth = depth;
  cmd.data = data;

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


void
opBufferTextureBind(opBuffer *buf, const opID id)
{
  // Did you try bind a different resource type?
  assert(op::type_id(id) == op::resource_type::TEXTURE);

  op::command::cmd_texture_bind cmd {};
  cmd.texture_id = op::instance_id(id);

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


// -- Target -- //


opID
opBufferTargetCreate(opContext *ctx,
                     opBuffer *buf,
                     opTargetDesc *in_out_desc)
{
  // -- Param Check -- //
  assert(ctx);
  assert(buf);
  assert(in_out_desc);

  // -- Generate Resource -- //
  op::context_data *ctx_data = &ctx->data;

  const opID new_target = ctx_data->add_target();

  if(op::resource_is_valid(new_target))
  {
    op::command::cmd_target_create cmd {};
    cmd.desc = in_out_desc;
    cmd.target_id = op::instance_id(new_target);

    buf->data.write_data((void*)&cmd, sizeof(cmd));
  }

  // -- Failed To Generate -- //
  return new_target;
}


void
opBufferTargetClear(opBuffer *buf,
                    const opID id,
                    const bool color,
                    const bool depth)
{
  // Did you try bind a different resource type?
  assert(op::type_id(id) == op::resource_type::TARGET);

  op::command::cmd_target_clear cmd {};
  cmd.target_id = op::instance_id(id);;
  cmd.color = color ? 1 : 0;
  cmd.depth = depth ? 1 : 0;

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


// -- Rasterizer -- //


opID
opBufferRasterizerCreate(opContext *ctx,
                         opBuffer *buf,
                         opRasterizerDesc *in_out_desc)
{
  // -- Param Check -- //
  assert(ctx);
  assert(buf);
  assert(in_out_desc);

  // -- Generate Resource -- //
  op::context_data *ctx_data = &ctx->data;

  const opID new_resource = ctx_data->add_rasterizer();

  if(op::resource_is_valid(new_resource))
  {
    op::command::cmd_rasterizer_create cmd {};
    cmd.desc = in_out_desc;
    cmd.rasterizer_id = op::instance_id(new_resource);

    buf->data.write_data((void*)&cmd, sizeof(cmd));
  }

  // -- Failed To Generate -- //
  return new_resource;
}


void
opBufferRasterizerBind(opBuffer *buf, const opID id)
{
  // Did you try bind a different resource type?
  assert(buf);
  assert(op::type_id(id) == op::resource_type::RASTERIZER);

  op::command::cmd_rasterizer_bind cmd {};
  cmd.rasterizer_id = op::instance_id(id);

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


// -- Blend -- //


opID
opBufferBlendCreate(opContext *ctx,
                    opBuffer *buf,
                    opBlendDesc *in_out_desc)
{
  // -- Param Check -- //
  assert(ctx);
  assert(buf);
  assert(in_out_desc);

  // -- Generate Resource -- //
  op::context_data *ctx_data = &ctx->data;

  const opID new_blend = ctx_data->add_blend();

  if(new_blend)
  {
    op::command::cmd_blend_create cmd {};
    cmd.desc = in_out_desc;
    cmd.blend_id = op::instance_id(new_blend);

    buf->data.write_data((void*)&cmd, sizeof(cmd));
  }

  return new_blend;
}


void
opBufferBlendBind(opBuffer *buf, const opID id)
{
  // -- Param Check -- //
  assert(buf);
  assert(op::type_id(id) == op::resource_type::BLEND);

  op::command::cmd_blend_bind cmd {};
  cmd.blend_id = op::instance_id(id);

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


// -- Render -- //


void
opBufferRender(opBuffer *buf)
{
  const op::command::cmd_render_generic cmd {};

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


void
opBufferRenderSubset(opBuffer *buf, const uint32_t start, const uint32_t end)
{
  assert(buf);

  op::command::cmd_render_subset cmd {};
  cmd.from = start;
  cmd.to   = end;

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


// -- Misc -- //


void
opBufferViewport(opBuffer *buf,
                 const size_t start_x,
                 const size_t start_y,
                 const size_t end_x,
                 const size_t end_y)
{
  assert(buf);

  op::command::cmd_viewport_bind cmd {};

  cmd.start_x = start_x;
  cmd.start_y = start_y;
  cmd.end_x   = end_x;
  cmd.end_y   = end_y;

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


// -- Debug -- //


void
opBufferDebugMarkerPush(opBuffer *buf, const char *name)
{
  // -- Param Check -- //
  assert(buf);

  op::command::cmd_debug_marker_push cmd {};

  cmd.name = name;

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


void
opBufferDebugMarkerPop(opBuffer *buf)
{
  // -- Param Check -- //
  assert(buf);

  op::command::cmd_debug_marker_pop cmd {};

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}
