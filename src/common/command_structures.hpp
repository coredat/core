#ifndef COMMAND_STRUCTURES_INCLUDED_104D6021_FB40_4D37_8B0F_48D3A560DF1F
#define COMMAND_STRUCTURES_INCLUDED_104D6021_FB40_4D37_8B0F_48D3A560DF1F


#include "../../include/op/desc.hpp"
#include <stdint.h>
#include <stddef.h>
#include "commands.hpp"


namespace op {
namespace command {


// -- Invalid/Generic command -- //


struct cmd_unknown {
  uint8_t id = command::NULL_CMD;
  uint8_t size = sizeof(cmd_unknown); // the size of this will vary as its unknown
};


// -- Device -- //


struct cmd_device_init {
  uint8_t id = command::DEVICE_INIT;
  uint8_t size = sizeof(cmd_device_init);
};


struct cmd_device_reset {
  uint8_t id = command::DEVICE_RESET;
  uint8_t size = sizeof(cmd_device_reset);
};


// -- Vertex format -- //


struct cmd_input_create {
  uint8_t id = command::VERTEX_FORMAT_CREATE;
  uint8_t size = sizeof(cmd_input_create);
  uint32_t vertex_fmt_id;
  opVertexDesc *desc;
  uint8_t desc_count;
};

struct cmd_input_bind {
  uint8_t id = command::VERTEX_FORMAT_BIND;
  uint8_t size = sizeof(cmd_input_bind);
  uint32_t vertex_fmt_id;
};


// -- Shaders -- //


struct cmd_shader_create {
  uint8_t id = command::SHADER_CREATE;
  uint8_t size = sizeof(cmd_shader_create);
  uint32_t shader_id;
  const char *vs;
  const char *gs;
  const char *ps;
  const char *name;
  opShaderDesc *desc;
};


struct cmd_shader_destroy {
  uint8_t id = command::SHADER_DESTROY;
  uint8_t size = sizeof(cmd_shader_destroy);
  uint32_t shader_id;
};


struct cmd_shader_bind {
  uint8_t id = command::SHADER_BIND;
  uint8_t size = sizeof(cmd_shader_bind);
  uint32_t shader_id;
};


// -- Shader Data -- //


struct cmd_shader_data_create {
  uint8_t id = command::SHADER_DATA_CREATE;
  uint8_t size = sizeof(cmd_shader_data_create);
  uint32_t shader_data_id;
  uint32_t shader_id;
  const char *name;
  opShaderDataDesc *desc;
};


struct cmd_shader_data_bind {
  uint8_t id = command::SHADER_DATA_BIND;
  uint8_t size = sizeof(cmd_shader_data_bind);
  uint32_t shader_data_id;
  uintptr_t data;
};


// -- Textures -- //


struct cmd_texture_create {
  uint8_t id = command::TEXTURE_CREATE;
  uint8_t size = sizeof(cmd_texture_create);
  uint32_t texture_id;
  void *data;
  opTextureDesc *desc;
};

struct cmd_texture_update {
  uint8_t id = command::TEXTURE_UPDATE;
  uint8_t size = sizeof(cmd_texture_update);
  uint32_t texture_id;
  void *data;
  size_t offset_x;
  size_t offset_y;
  size_t offset_z;
  size_t width;
  size_t height;
  size_t depth;
};

struct cmd_texture_bind {
  uint8_t id = command::TEXTURE_BIND;
  uint8_t size = sizeof(cmd_texture_bind);
  uint32_t texture_id;
};


// -- Texture Filtering -- //

struct cmd_texture_filter_create {
  uint8_t id = command::TEXTURE_FILTER_CREATE;
  uint8_t size = sizeof(cmd_texture_filter_create);
  uint32_t texture_filter_id;
  opTextureFilterDesc *desc;
};


struct cmd_texture_filter_bind {
  uint8_t id = command::TEXTURE_FILTER_BIND;
  uint8_t size = sizeof(cmd_texture_filter_bind);
  uint32_t texture_filter_id;
};

// -- Buffer -- //


// -- Geometry -- //


struct cmd_geometry_create {
  uint8_t id = command::GEOMETRY_CREATE;
  uint8_t size = sizeof(cmd_geometry_create);
  uint32_t geometry_id;
  void *data;
  size_t sizeof_data;
  size_t element_count;
  opGeometryDesc *desc;
};

struct cmd_geometry_bind {
  uint8_t id = command::GEOMETRY_BIND;
  uint8_t size = sizeof(cmd_geometry_bind);
  uint32_t geometry_id;
};


// -- Index -- //

struct cmd_index_create {
  uint8_t id = command::INDEX_CREATE;
  uint8_t size = sizeof(cmd_index_create);
  uint32_t index_id;
  void *data;
  size_t data_bytes;
  size_t element_count;
  opIndexDesc *desc;
};

struct cmd_index_bind {
  uint8_t id = command::INDEX_BIND;
  uint8_t size = sizeof(cmd_index_bind);
  uint32_t index_id;
};


// -- Target -- //


struct cmd_target_create {
  uint8_t id = command::TARGET_CREATE;
  uint8_t size = sizeof(cmd_target_create);
  uint32_t target_id;
  opTargetDesc *desc;
};


struct cmd_target_clear {
  uint8_t id = command::TARGET_CLEAR;
  uint8_t size = sizeof(cmd_target_clear);
  uint32_t target_id;
  uint8_t color;
  uint8_t depth;
};


struct cmd_target_update {
  uint8_t id = command::TARGET_UPDATE;
  uint8_t size = sizeof(cmd_target_update);
  opTargetDesc *desc;
};


// -- Rasterizer -- //


struct cmd_rasterizer_create {
  uint8_t id = command::RASTERIZER_CREATE;
  uint8_t size = sizeof(cmd_rasterizer_create);
  uint32_t rasterizer_id;
  opRasterizerDesc *desc;
};


struct cmd_rasterizer_update {
  uint8_t id = command::RASTERIZER_UPDATE;
  uint8_t size = sizeof(cmd_rasterizer_update);
  uint32_t rasterizer_id;
  opRasterizerDesc *desc;
};


struct cmd_rasterizer_destroy {
  uint8_t id = command::RASTERIZER_DESTROY;
  uint8_t size = sizeof(cmd_rasterizer_destroy);
  uint32_t rasterizer_id;
};


struct cmd_rasterizer_bind {
  uint8_t id = command::RASTERIZER_BIND;
  uint8_t size = sizeof(cmd_rasterizer_bind);
  uint32_t rasterizer_id;
};


// -- Render -- //


struct cmd_render_generic {
  uint8_t id = command::RENDER_GENERIC;
  uint8_t size = sizeof(cmd_render_generic);
};


struct cmd_render_subset {
  uint8_t id = command::RENDER_SUBSET;
  uint8_t size = sizeof(cmd_render_generic);
  uint32_t from;
  uint32_t to;
};


// -- Misc -- //

struct cmd_viewport_bind {
  uint8_t id = command::VIEWPORT_BIND;
  uint8_t size = sizeof(cmd_viewport_bind);
  size_t start_x;
  size_t start_y;
  size_t end_x;
  size_t end_y;
};


// -- Debug -- //

struct cmd_debug_marker_push {
  uint8_t id = command::DEBUG_MARKER_PUSH;
  uint8_t size = sizeof(cmd_debug_marker_push);
  const char *name;
};


struct cmd_debug_marker_pop {
  uint8_t id = command::DEBUG_MARKER_POP;
  uint8_t size = sizeof(cmd_debug_marker_pop);
};


} // ns
} // ns


#endif // inc guard
