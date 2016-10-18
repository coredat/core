
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef TEXTURE_INCLUDED_67763631_F566_4F4A_878F_B3AB382B6844
#define TEXTURE_INCLUDED_67763631_F566_4F4A_878F_B3AB382B6844


#include <stdint.h>
#include <stddef.h>
#include <graphics_api/ogl/ogl_texture.hpp>
#include <graphics_api/ogl/ogl_frame_buffer.hpp>


namespace Data {


/*!
  Texture Model
*/
struct Texture_data
{
  // key //
  uint32_t *keys = nullptr;

  // name field data //
  char *field_name = nullptr;

  // texture field data //
  Ogl::Texture *field_texture = nullptr;

  // render_target field data //
  Ogl::Frame_buffer *field_render_target = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Texture Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Texture_data *data);


void
data_unlock(const Texture_data *data);


bool
texture_create(Texture_data *data, const size_t size_hint);


void
texture_destroy(Texture_data *data);


uint32_t
texture_push(Texture_data *data);


bool
texture_remove(Texture_data *data, const uint32_t key);


bool
texture_exists(const Texture_data *data, const uint32_t key, size_t *out_index = nullptr);


void
texture_clear(Texture_data *data);


bool
texture_is_empty(const Texture_data *data);


size_t
texture_get_size(const Texture_data *data);


size_t
texture_get_capacity(const Texture_data *data);


bool
texture_resize_capacity(Texture_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
texture_get_const_name_data(const Texture_data *data);


char*
texture_get_name_data(Texture_data *data);


const Ogl::Texture*
texture_get_const_texture_data(const Texture_data *data);


Ogl::Texture*
texture_get_texture_data(Texture_data *data);


const Ogl::Frame_buffer*
texture_get_const_render_target_data(const Texture_data *data);


Ogl::Frame_buffer*
texture_get_render_target_data(Texture_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
texture_get_name(const Texture_data *data, const uint32_t key, const char **return_value);


bool
texture_set_name(const Texture_data *data, const uint32_t key, const char *set_value, const size_t size);


bool
texture_get_texture(const Texture_data *data, const uint32_t key, Ogl::Texture *return_value);


bool
texture_set_texture(const Texture_data *data, const uint32_t key, const Ogl::Texture *set_value);


bool
texture_get_render_target(const Texture_data *data, const uint32_t key, Ogl::Frame_buffer *return_value);


bool
texture_set_render_target(const Texture_data *data, const uint32_t key, const Ogl::Frame_buffer *set_value);


} // Data ns


#endif // inc guard
