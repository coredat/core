
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef MATERIAL_INCLUDED_C573BB0D_E8FE_4BEF_8537_AA2D3C8748EA
#define MATERIAL_INCLUDED_C573BB0D_E8FE_4BEF_8537_AA2D3C8748EA


#include <stdint.h>
#include <stddef.h>
#include <systems/renderer_material/material.hpp>


namespace Data {


/*!
  Material Model
*/
struct Material_data
{
  // key //
  uint32_t *keys = nullptr;

  // name field data //
  char *field_name = nullptr;

  // material_hash field data //
  Material_renderer::Material_id *field_material_hash = nullptr;

  // material field data //
  Material_renderer::Material *field_material = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Material Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Material_data *data);


void
data_unlock(const Material_data *data);


bool
material_create(Material_data *data, const size_t size_hint);


void
material_destroy(Material_data *data);


uint32_t
material_push(Material_data *data);


bool
material_remove(Material_data *data, const uint32_t key);


bool
material_exists(const Material_data *data, const uint32_t key, size_t *out_index = nullptr);


void
material_clear(Material_data *data);


bool
material_is_empty(const Material_data *data);


size_t
material_get_size(const Material_data *data);


size_t
material_get_capacity(const Material_data *data);


bool
material_resize_capacity(Material_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
material_get_const_name_data(const Material_data *data);


char*
material_get_name_data(Material_data *data);


const Material_renderer::Material_id*
material_get_const_material_hash_data(const Material_data *data);


Material_renderer::Material_id*
material_get_material_hash_data(Material_data *data);


const Material_renderer::Material*
material_get_const_material_data(const Material_data *data);


Material_renderer::Material*
material_get_material_data(Material_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
material_get_name(const Material_data *data, const uint32_t key, const char **return_value);


bool
material_set_name(const Material_data *data, const uint32_t key, const char *set_value, const size_t size);


constexpr size_t
material_get_name_stride() { return 32; }


bool
material_get_material_hash(const Material_data *data, const uint32_t key, Material_renderer::Material_id *return_value);


bool
material_set_material_hash(const Material_data *data, const uint32_t key, const Material_renderer::Material_id *set_value);




bool
material_get_material(const Material_data *data, const uint32_t key, Material_renderer::Material *return_value);


bool
material_set_material(const Material_data *data, const uint32_t key, const Material_renderer::Material *set_value);




} // Data ns


#endif // inc guard
