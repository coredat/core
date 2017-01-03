#include "buffer_impl.hpp"
#include "context_impl.hpp"

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

allocCallback   alloc_cb   = nullptr;
reallocCallback realloc_cb = nullptr;
freeCallback    free_cb    = nullptr;
uintptr_t       user_data  = 0;

} // anon ns



void
opBufferAllocCallback(const allocCallback cb)
{
  alloc_cb = cb;
}


void
opBufferResizeCallback(const reallocCallback cb)
{
  realloc_cb = cb;
}


void
opBufferDestroyCallback(const freeCallback cb)
{
  free_cb = cb;
}


void
opBufferUserData(const uintptr_t ud)
{
  user_data = ud;
}


opBuffer*
opBufferCreate()
{
  opBuffer *buffer = new opBuffer;

  buffer->data.alloc_cb   = alloc_cb;
  buffer->data.realloc_cb = realloc_cb;
  buffer->data.destroy_cb = free_cb;
  buffer->data.user_data  = user_data;
  buffer->data.initial_reserve();

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

  return {0,0};
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
  cmd.vertex_fmt_id = new_vertex_format.instance;
  cmd.desc          = desc;
  cmd.desc_count    = attr_count;

  buf->data.write_data((void*)&cmd, sizeof(cmd));

  return (new_vertex_format);
}


void
opBufferVertexFormatBind(opBuffer *buf, const opID id)
{
  // Did you try bind a different resource type?
  assert(id.type == op::resource_type::VERTEX_FORMAT);

  op::command::cmd_input_bind cmd {};
  cmd.vertex_fmt_id = id.instance;

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

  if(resource_is_valid(new_shader))
  {
    op::command::cmd_shader_create cmd {};
    cmd.vs   = vs;
    cmd.gs   = gs;
    cmd.ps   = ps;
    cmd.name = name;
    cmd.desc = in_out_desc;

    buf->data.write_data((void*)&cmd, sizeof(cmd));

    return (new_shader);
  }

  // -- Failed To Generate -- //
  return (op::invalid_resource_id());
}


void
opBufferShaderBind(opBuffer *buf, const opID id)
{
  // Did you try bind a different resource type?
  assert(id.type == op::resource_type::SHADER);

  op::command::cmd_shader_bind cmd {};
  cmd.shader_id = id.instance;

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

  if(resource_is_valid(new_shader_data))
  {
    op::command::cmd_shader_data_create cmd {};
    cmd.shader_data_id = new_shader_data.instance;
    cmd.shader_id      = shader_id.instance;
    cmd.name           = name;
    cmd.desc           = in_out_desc;

    buf->data.write_data((void*)&cmd, sizeof(cmd));

    return (new_shader_data);
  }

  // -- Failed To Generate -- //
  return (op::invalid_resource_id());
}


void
opBufferShaderDataBind(opBuffer *buf,
                       const opID id,
                       void *data)
{
  // Did you try bind a different resource type?
  assert(id.type == op::resource_type::SHADER_DATA);

  op::command::cmd_shader_data_bind cmd {};
  cmd.shader_data_id = id.instance;
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
  assert(id.type == op::resource_type::SHADER_DATA);
  assert(texture_id.type == op::resource_type::TEXTURE);

  op::command::cmd_shader_data_bind cmd {};
  cmd.shader_data_id = id.instance;
  cmd.data           = (uintptr_t)id.instance;

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

  if(resource_is_valid(new_geom))
  {
    op::command::cmd_geometry_create cmd {};
    cmd.desc          = in_out_desc;
    cmd.geometry_id   = new_geom.instance;
    cmd.sizeof_data   = bytes_of_data;
    cmd.element_count = element_count;
    cmd.data          = data;

    buf->data.write_data((void*)&cmd, sizeof(cmd));

    return (new_geom);
  }

  // -- Failed To Generate -- //
  return (op::invalid_resource_id());
}


void
opBufferGeometryBind(opBuffer *buf, const opID id)
{
  // Did you try bind a different resource type?
  assert(id.type == op::resource_type::GEOMETRY);

  op::command::cmd_geometry_bind cmd {};
  cmd.geometry_id = id.instance;

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

  if(resource_is_valid(new_index))
  {
    op::command::cmd_index_create cmd {};
    cmd.desc = in_out_desc;
    cmd.data = data;
    cmd.data_bytes = bytes_of_data;
    cmd.element_count = element_count;
    cmd.index_id = new_index.instance;

    buf->data.write_data((void*)&cmd, sizeof(cmd));

    return new_index;
  }

  // -- Failed To Generate -- //
  return op::invalid_resource_id();
}


void
opBufferIndexBind(opBuffer *buf, const opID id)
{
  // Did you try bind a different resource type?
  assert(id.type == op::resource_type::INDEX);

  op::command::cmd_index_bind cmd {};
  cmd.index_id = id.instance;

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

  if(resource_is_valid(new_texture))
  {
    op::command::cmd_texture_create cmd {};
    cmd.data = data;
    cmd.desc = in_out_desc;

    buf->data.write_data((void*)&cmd, sizeof(cmd));

    return new_texture;
  }

  // -- Failed To Generate -- //
  return op::invalid_resource_id();
}


void
opBufferTextureBind(opBuffer *buf, const opID id)
{
  // Did you try bind a different resource type?
  assert(id.type == op::resource_type::TEXTURE);

  op::command::cmd_texture_bind cmd {};
  cmd.texture_id = id.instance;

  buf->data.write_data((void*)&cmd, sizeof(cmd));
}


// -- Texture Filter -- //


opID
opBufferTextureFilterCreate(opContext *ctx,
                            opBuffer *buf,
                            opTextureFilterDesc *in_out_desc)
{
  // -- Param Check -- //
  assert(ctx);
  assert(buf);
  assert(in_out_desc);

  // -- Generate Resource -- //
  op::context_data *ctx_data = &ctx->data;

  const opID new_filter = ctx_data->add_texture_filter();

  if(resource_is_valid(new_filter))
  {
    op::command::cmd_texture_filter_create cmd {};
    cmd.desc = in_out_desc;

    buf->data.write_data((void*)&cmd, sizeof(cmd));

    return new_filter;
  }

  // -- Failed To Generate -- //
  return op::invalid_resource_id();
}


void
opBufferTextureFilterBind(opBuffer *buf, opID id)
{
  // Did you try bind a different resource type?
  assert(id.type == op::resource_type::TEXTURE_FILTER);

  op::command::cmd_texture_filter_bind cmd {};
  cmd.texture_filter_id = id.instance;

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

  if(resource_is_valid(new_target))
  {
    op::command::cmd_target_create cmd {};
    cmd.desc = in_out_desc;
    cmd.target_id = new_target.instance;

    buf->data.write_data((void*)&cmd, sizeof(cmd));

    return new_target;
  }

  // -- Failed To Generate -- //
  return op::invalid_resource_id();
}


void
opBufferTargetClear(opBuffer *buf,
                    const opID id,
                    const bool color,
                    const bool depth)
{
  // Did you try bind a different resource type?
  assert(id.type == op::resource_type::TARGET);

  op::command::cmd_target_clear cmd {};
  cmd.target_id = id.instance;
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

  if(resource_is_valid(new_resource))
  {
    op::command::cmd_rasterizer_create cmd {};
    cmd.desc = in_out_desc;
    cmd.rasterizer_id = new_resource.instance;

    buf->data.write_data((void*)&cmd, sizeof(cmd));

    return new_resource;
  }

  // -- Failed To Generate -- //
  return op::invalid_resource_id();
}


void
opBufferRasterizerBind(opBuffer *buf, const opID id)
{
  // Did you try bind a different resource type?
  assert(id.type == op::resource_type::RASTERIZER);

  op::command::cmd_rasterizer_bind cmd {};
  cmd.rasterizer_id = id.instance;

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

  op::command::cmd_render_subset cmd{};
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
