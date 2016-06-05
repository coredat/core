
/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/

#ifndef RENDERER_MESH_DATA_INCLUDED_FDEC262C_335B_419E_BC37_5DF37AC77BD4
#define RENDERER_MESH_DATA_INCLUDED_FDEC262C_335B_419E_BC37_5DF37AC77BD4


#include <utilities/generic_id.hpp>
#include <utilities/memory_pool.hpp>
#include <stddef.h>


namespace World_data {


struct Mesh_renderer_draw_call {
  uint32_t model;
  uint32_t texture; // Move this to the material.
  float    world_matrix[16];
};



struct Renderer_mesh_data
{
  // Data Key
  util::generic_id *data_key = nullptr;

  // Properties
  Mesh_renderer_draw_call *property_draw_call = nullptr;

  // Size information
  size_t size = 0;
  const size_t capacity = 0;

  // Memory chunk
  const util::memory_chunk memory = util::memory_chunk();

}; // struct


void
renderer_mesh_data_init(Renderer_mesh_data *data, const size_t size_hint);


void
renderer_mesh_data_free(Renderer_mesh_data *data);


size_t
renderer_mesh_data_get_size(const Renderer_mesh_data *data);


size_t
renderer_mesh_data_get_capacity(const Renderer_mesh_data *data);


void
data_lock(Renderer_mesh_data *data);


void
data_unlock(Renderer_mesh_data *data);


bool
renderer_mesh_data_push_back(Renderer_mesh_data *data, const util::generic_id key, size_t *out_index = nullptr);


bool
renderer_mesh_data_erase(Renderer_mesh_data *data, const util::generic_id key);


bool
renderer_mesh_data_exists(const Renderer_mesh_data *data, const util::generic_id key, size_t *out_index = nullptr);


bool
renderer_mesh_data_get_property_draw_call(const Renderer_mesh_data *data, const util::generic_id key, Mesh_renderer_draw_call **value);


bool
renderer_mesh_data_set_property_draw_call(Renderer_mesh_data *data,  const util::generic_id key, const Mesh_renderer_draw_call *value);


} // ns


#endif // inc guard
