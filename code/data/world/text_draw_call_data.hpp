
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef TEXT_DRAW_CALL_INCLUDED_EAACCA5C_02FA_433C_9AE5_F66C669905D6
#define TEXT_DRAW_CALL_INCLUDED_EAACCA5C_02FA_433C_9AE5_F66C669905D6


#include <stdint.h>
#include <stddef.h>
#include <systems/renderer_text/text_draw_call.hpp>
#include <utilities/generic_id.hpp>


namespace Data {


/*!
  Text_draw_call Model
*/
struct Text_draw_call_data
{
  // key //
  uint32_t *keys = nullptr;

  // text field data //
  char *field_text = nullptr;

  // draw_call field data //
  Text_renderer::Draw_call *field_draw_call = nullptr;

  // model_id field data //
  util::generic_id *field_model_id = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Text_draw_call Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Text_draw_call_data *data);


void
data_unlock(const Text_draw_call_data *data);


bool
text_draw_call_create(Text_draw_call_data *data, const size_t size_hint);


void
text_draw_call_destroy(Text_draw_call_data *data);


uint32_t
text_draw_call_push(Text_draw_call_data *data, const uint32_t key);


bool
text_draw_call_insert(Text_draw_call_data *data, const uint32_t key, const size_t insert_point);


bool
text_draw_call_remove(Text_draw_call_data *data, const uint32_t key);


bool
text_draw_call_exists(const Text_draw_call_data *data, const uint32_t key, size_t *out_index = nullptr);


void
text_draw_call_clear(Text_draw_call_data *data);


bool
text_draw_call_is_empty(const Text_draw_call_data *data);


size_t
text_draw_call_get_size(const Text_draw_call_data *data);


size_t
text_draw_call_get_capacity(const Text_draw_call_data *data);


bool
text_draw_call_resize_capacity(Text_draw_call_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
text_draw_call_get_const_text_data(const Text_draw_call_data *data);


char*
text_draw_call_get_text_data(Text_draw_call_data *data);


const Text_renderer::Draw_call*
text_draw_call_get_const_draw_call_data(const Text_draw_call_data *data);


Text_renderer::Draw_call*
text_draw_call_get_draw_call_data(Text_draw_call_data *data);


const util::generic_id*
text_draw_call_get_const_model_id_data(const Text_draw_call_data *data);


util::generic_id*
text_draw_call_get_model_id_data(Text_draw_call_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
text_draw_call_get_text(const Text_draw_call_data *data, const uint32_t key, const char **return_value);


bool
text_draw_call_set_text(const Text_draw_call_data *data, const uint32_t key, const char *set_value, const size_t size);


bool
text_draw_call_get_draw_call(const Text_draw_call_data *data, const uint32_t key, Text_renderer::Draw_call *return_value);


bool
text_draw_call_set_draw_call(const Text_draw_call_data *data, const uint32_t key, const Text_renderer::Draw_call *set_value);


bool
text_draw_call_get_model_id(const Text_draw_call_data *data, const uint32_t key, util::generic_id *return_value);


bool
text_draw_call_set_model_id(const Text_draw_call_data *data, const uint32_t key, const util::generic_id *set_value);


} // Data ns


#endif // inc guard
