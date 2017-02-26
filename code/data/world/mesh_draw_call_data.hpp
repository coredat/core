
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef MESH_DRAW_CALL_INCLUDED_9ED4EA1E_6152_4A8B_8F48_A374D1F05AB7
#define MESH_DRAW_CALL_INCLUDED_9ED4EA1E_6152_4A8B_8F48_A374D1F05AB7


#include <stdint.h>
#include <stddef.h>
#include "todo.hpp"


namespace Data {


/*!
  Mesh_draw_call Model
*/
struct Mesh_draw_call_data
{
  // key //
  uint32_t *keys = nullptr;

  // draw_call field data //
  Mesh_renderer_draw_call *field_draw_call = nullptr;

  // material_id field data //
  uint32_t *field_material_id = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Mesh_draw_call Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Mesh_draw_call_data *data);


void
data_unlock(const Mesh_draw_call_data *data);


bool
mesh_draw_call_create(Mesh_draw_call_data *data, const size_t size_hint);


void
mesh_draw_call_destroy(Mesh_draw_call_data *data);


uint32_t
mesh_draw_call_push(Mesh_draw_call_data *data, const uint32_t key);


bool
mesh_draw_call_insert(Mesh_draw_call_data *data, const uint32_t key, const size_t insert_point);


bool
mesh_draw_call_remove(Mesh_draw_call_data *data, const uint32_t key);


bool
mesh_draw_call_exists(const Mesh_draw_call_data *data, const uint32_t key, size_t *out_index = nullptr);


void
mesh_draw_call_clear(Mesh_draw_call_data *data);


bool
mesh_draw_call_is_empty(const Mesh_draw_call_data *data);


size_t
mesh_draw_call_get_size(const Mesh_draw_call_data *data);


size_t
mesh_draw_call_get_capacity(const Mesh_draw_call_data *data);


bool
mesh_draw_call_resize_capacity(Mesh_draw_call_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const Mesh_renderer_draw_call*
mesh_draw_call_get_const_draw_call_data(const Mesh_draw_call_data *data);


Mesh_renderer_draw_call*
mesh_draw_call_get_draw_call_data(Mesh_draw_call_data *data);


const uint32_t*
mesh_draw_call_get_const_material_id_data(const Mesh_draw_call_data *data);


uint32_t*
mesh_draw_call_get_material_id_data(Mesh_draw_call_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
mesh_draw_call_get_draw_call(const Mesh_draw_call_data *data, const uint32_t key, Mesh_renderer_draw_call *return_value);


bool
mesh_draw_call_set_draw_call(const Mesh_draw_call_data *data, const uint32_t key, const Mesh_renderer_draw_call *set_value);




bool
mesh_draw_call_get_material_id(const Mesh_draw_call_data *data, const uint32_t key, uint32_t *return_value);


bool
mesh_draw_call_set_material_id(const Mesh_draw_call_data *data, const uint32_t key, const uint32_t *set_value);




} // Data ns


#endif // inc guard
