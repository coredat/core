
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef TEXT_MESH_INCLUDED_9B142F8A_D441_4450_9356_6DB95AC09255
#define TEXT_MESH_INCLUDED_9B142F8A_D441_4450_9356_6DB95AC09255


#include <stdint.h>
#include <stddef.h>
#include <utilities/generic_id.hpp>
#include <graphics_api/mesh.hpp>


namespace Data {


/*!
  Text_mesh Model
*/
struct Text_mesh_data
{
  // key //
  uint32_t *keys = nullptr;

  // text field data //
  char *field_text = nullptr;

  // text_size field data //
  uint32_t *field_text_size = nullptr;

  // font_id field data //
  util::generic_id *field_font_id = nullptr;

  // mesh field data //
  Graphics_api::Mesh *field_mesh = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Text_mesh Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Text_mesh_data *data);


void
data_unlock(const Text_mesh_data *data);


bool
text_mesh_create(Text_mesh_data *data, const size_t size_hint);


void
text_mesh_destroy(Text_mesh_data *data);


uint32_t
text_mesh_push(Text_mesh_data *data);


bool
text_mesh_remove(Text_mesh_data *data, const uint32_t key);


bool
text_mesh_exists(const Text_mesh_data *data, const uint32_t key, size_t *out_index = nullptr);


void
text_mesh_clear(Text_mesh_data *data);


bool
text_mesh_is_empty(const Text_mesh_data *data);


size_t
text_mesh_get_size(const Text_mesh_data *data);


size_t
text_mesh_get_capacity(const Text_mesh_data *data);


bool
text_mesh_resize_capacity(Text_mesh_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
text_mesh_get_const_text_data(const Text_mesh_data *data);


char*
text_mesh_get_text_data(Text_mesh_data *data);


const uint32_t*
text_mesh_get_const_text_size_data(const Text_mesh_data *data);


uint32_t*
text_mesh_get_text_size_data(Text_mesh_data *data);


const util::generic_id*
text_mesh_get_const_font_id_data(const Text_mesh_data *data);


util::generic_id*
text_mesh_get_font_id_data(Text_mesh_data *data);


const Graphics_api::Mesh*
text_mesh_get_const_mesh_data(const Text_mesh_data *data);


Graphics_api::Mesh*
text_mesh_get_mesh_data(Text_mesh_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
text_mesh_get_text(const Text_mesh_data *data, const uint32_t key, const char **return_value);


bool
text_mesh_set_text(const Text_mesh_data *data, const uint32_t key, const char *set_value, const size_t size);


constexpr size_t
text_mesh_get_text_stride() { return 32; }


bool
text_mesh_get_text_size(const Text_mesh_data *data, const uint32_t key, uint32_t *return_value);


bool
text_mesh_set_text_size(const Text_mesh_data *data, const uint32_t key, const uint32_t *set_value);




bool
text_mesh_get_font_id(const Text_mesh_data *data, const uint32_t key, util::generic_id *return_value);


bool
text_mesh_set_font_id(const Text_mesh_data *data, const uint32_t key, const util::generic_id *set_value);




bool
text_mesh_get_mesh(const Text_mesh_data *data, const uint32_t key, Graphics_api::Mesh *return_value);


bool
text_mesh_set_mesh(const Text_mesh_data *data, const uint32_t key, const Graphics_api::Mesh *set_value);




} // Data ns


#endif // inc guard
