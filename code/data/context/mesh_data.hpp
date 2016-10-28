
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef MESH_INCLUDED_A65EFD89_C268_4052_84A1_BE8974C1EB0D
#define MESH_INCLUDED_A65EFD89_C268_4052_84A1_BE8974C1EB0D


#include <stdint.h>
#include <stddef.h>
#include <graphics_api/mesh.hpp>
#include <math/geometry/geometry_types.hpp>


namespace Data {


/*!
  Mesh Model
*/
struct Mesh_data
{
  // key //
  uint32_t *keys = nullptr;

  // name field data //
  char *field_name = nullptr;

  // mesh field data //
  Graphics_api::Mesh *field_mesh = nullptr;

  // aabb field data //
  math::aabb *field_aabb = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Mesh Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Mesh_data *data);


void
data_unlock(const Mesh_data *data);


bool
mesh_create(Mesh_data *data, const size_t size_hint);


void
mesh_destroy(Mesh_data *data);


uint32_t
mesh_push(Mesh_data *data);


bool
mesh_remove(Mesh_data *data, const uint32_t key);


bool
mesh_exists(const Mesh_data *data, const uint32_t key, size_t *out_index = nullptr);


void
mesh_clear(Mesh_data *data);


bool
mesh_is_empty(const Mesh_data *data);


size_t
mesh_get_size(const Mesh_data *data);


size_t
mesh_get_capacity(const Mesh_data *data);


bool
mesh_resize_capacity(Mesh_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
mesh_get_const_name_data(const Mesh_data *data);


char*
mesh_get_name_data(Mesh_data *data);


const Graphics_api::Mesh*
mesh_get_const_mesh_data(const Mesh_data *data);


Graphics_api::Mesh*
mesh_get_mesh_data(Mesh_data *data);


const math::aabb*
mesh_get_const_aabb_data(const Mesh_data *data);


math::aabb*
mesh_get_aabb_data(Mesh_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
mesh_get_name(const Mesh_data *data, const uint32_t key, const char **return_value);


bool
mesh_set_name(const Mesh_data *data, const uint32_t key, const char *set_value, const size_t size);


constexpr size_t
mesh_get_name_stride() { return 32; }


bool
mesh_get_mesh(const Mesh_data *data, const uint32_t key, Graphics_api::Mesh *return_value);


bool
mesh_set_mesh(const Mesh_data *data, const uint32_t key, const Graphics_api::Mesh *set_value);




bool
mesh_get_aabb(const Mesh_data *data, const uint32_t key, math::aabb *return_value);


bool
mesh_set_aabb(const Mesh_data *data, const uint32_t key, const math::aabb *set_value);




} // Data ns


#endif // inc guard
