#ifndef CONTEXT_DATA_INCLUDED_DEB383AD_E624_4632_BE2B_6CE4DA4883A1
#define CONTEXT_DATA_INCLUDED_DEB383AD_E624_4632_BE2B_6CE4DA4883A1


#include "../common/common.hpp"
#include "../api/ogl/descs/fwd.hpp"
#include "../../include/op/common/resource_id.hpp"

#include <stdarg.h>
#include <stdio.h>

namespace op {

/*
  TODO: Sink this into OGL making it OGL Specific.
*/
struct context_data
{
  // -- Data -- //

  API::shader_desc *shader_descs                   = nullptr;
  size_t shader_desc_capacity                      = 0;
  size_t shader_desc_size                          = 0;
  size_t shader_last_bound                         = 0;
  bool   shader_currently_bound                    = false;

  API::uniform_desc *uniform_descs                 = nullptr;
  size_t uniform_desc_capacity                     = 0;
  size_t uniform_desc_size                         = 0;
  size_t uniform_last_bound                        = 0;

  API::shader_data_desc *shader_data_descs         = nullptr;
  size_t shader_data_desc_capacity                 = 0;
  size_t shader_data_desc_size                     = 0;
  size_t shader_data_last_bound                    = 0;

  API::target_desc *target_descs                   = nullptr;
  size_t target_desc_capacity                      = 0;
  size_t target_desc_size                          = 0;
  size_t target_last_bound                         = 0;

  API::vertex_format_desc *vertex_format_descs     = nullptr;
  size_t vertex_format_desc_capacity               = 0;
  size_t vertex_format_desc_size                   = 0;
  size_t vertex_format_last_bind                   = 0;
  bool   vertex_format_currently_bound             = false;

  API::geometry_desc *geometry_descs               = nullptr;
  size_t geometry_desc_capacity                    = 0;
  size_t geometry_desc_size                        = 0;
  size_t geometry_last_bind                        = 0;
  bool   geometry_currently_bound                  = false;

  API::index_internal_desc *index_descs            = nullptr;
  size_t index_desc_capacity                       = 0;
  size_t index_desc_size                           = 0;
  size_t index_last_bind                           = 0;
  bool   index_currently_bound                     = false;

  API::rasterizer_desc *rasterizer_descs           = nullptr;
  size_t rasterizer_desc_capacity                  = 0;
  size_t rasterizer_desc_size                      = 0;
  size_t rasterizer_last_bind                      = 0;
  bool   rasterizer_currently_bound                = false;

  API::texture_desc *texture_descs                 = nullptr;
  size_t texture_desc_capacity                     = 0;
  size_t texture_desc_size                         = 0;
  size_t texture_last_bind                         = 0;
  bool   texture_currently_bound                   = false;

  API::texture_filtering_internal_desc *texture_filtering_descs = nullptr;
  size_t texture_filtering_desc_capacity           = 0;
  size_t texture_filtering_desc_size               = 0;
  size_t texture_filtering_last_bind               = 0;
  bool   texture_filtering_currently_bound         = false;

  // -- Callbacks -- //

  allocCallback     alloc_cb;
  reallocCallback   realloc_cb;
  freeCallback      destroy_cb;
  uintptr_t         user_data;

  // -- Methods -- //

  void              initial_reserve();
  uint32_t          add_shader();
  uint32_t          add_shader_data();
  uint32_t          add_target();
  uint32_t          add_texture();
  uint32_t          add_geometry();
  uint32_t          add_index();
  uint32_t          add_vertex_format();
  uint32_t          add_rasterizer();
  uint32_t          add_texture_filter();

  // -- Alloc / Free -- //

  void*             alloc(const size_t bytes);
  void              free(void *);

  // -- Some Logging Helpers -- //

  void              vlog(const char *output, va_list args);
  void              log(const char *output, ...);

  // -- Support Data -- //

  uint32_t          texture_1D_max_width;
  uint32_t          texture_2D_max_width;
  uint32_t          texture_2D_max_height;
  uint32_t          texture_3D_max_width;
  uint32_t          texture_3D_max_height;
  uint32_t          texture_3D_max_depth;
  float             anisotropic_max_multiplier;

};


} // ns


#endif // inc guard
