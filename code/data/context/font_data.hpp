
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef FONT_INCLUDED_2BEE8053_C39C_4227_A42D_94527A8B0584
#define FONT_INCLUDED_2BEE8053_C39C_4227_A42D_94527A8B0584


#include <stdint.h>
#include <stddef.h>
#include <3rdparty/stb/stb_truetype.h>
#include <systems/text/font_bitmap.hpp>


namespace Data {


/*!
  Font Model
*/
struct Font_data
{
  // key //
  uint32_t *keys = nullptr;

  // name field data //
  char *field_name = nullptr;

  // font_face field data //
  stbtt_fontinfo *field_font_face = nullptr;

  // font_bitmap field data //
  Text::Font_bitmap *field_font_bitmap = nullptr;

  // glyph_texture_id field data //
  uint32_t *field_glyph_texture_id = nullptr;

  // metric_texture_id field data //
  uint32_t *field_metric_texture_id = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Font Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Font_data *data);


void
data_unlock(const Font_data *data);


bool
font_create(Font_data *data, const size_t size_hint);


void
font_destroy(Font_data *data);


uint32_t
font_push(Font_data *data);


bool
font_remove(Font_data *data, const uint32_t key);


bool
font_exists(const Font_data *data, const uint32_t key, size_t *out_index = nullptr);


void
font_clear(Font_data *data);


bool
font_is_empty(const Font_data *data);


size_t
font_get_size(const Font_data *data);


size_t
font_get_capacity(const Font_data *data);


bool
font_resize_capacity(Font_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const char*
font_get_const_name_data(const Font_data *data);


char*
font_get_name_data(Font_data *data);


const stbtt_fontinfo*
font_get_const_font_face_data(const Font_data *data);


stbtt_fontinfo*
font_get_font_face_data(Font_data *data);


const Text::Font_bitmap*
font_get_const_font_bitmap_data(const Font_data *data);


Text::Font_bitmap*
font_get_font_bitmap_data(Font_data *data);


const uint32_t*
font_get_const_glyph_texture_id_data(const Font_data *data);


uint32_t*
font_get_glyph_texture_id_data(Font_data *data);


const uint32_t*
font_get_const_metric_texture_id_data(const Font_data *data);


uint32_t*
font_get_metric_texture_id_data(Font_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
font_get_name(const Font_data *data, const uint32_t key, const char **return_value);


bool
font_set_name(const Font_data *data, const uint32_t key, const char *set_value, const size_t size);


constexpr size_t
font_get_name_stride() { return 32; }


bool
font_get_font_face(const Font_data *data, const uint32_t key, stbtt_fontinfo *return_value);


bool
font_set_font_face(const Font_data *data, const uint32_t key, const stbtt_fontinfo *set_value);




bool
font_get_font_bitmap(const Font_data *data, const uint32_t key, Text::Font_bitmap *return_value);


bool
font_set_font_bitmap(const Font_data *data, const uint32_t key, const Text::Font_bitmap *set_value);




bool
font_get_glyph_texture_id(const Font_data *data, const uint32_t key, uint32_t *return_value);


bool
font_set_glyph_texture_id(const Font_data *data, const uint32_t key, const uint32_t *set_value);




bool
font_get_metric_texture_id(const Font_data *data, const uint32_t key, uint32_t *return_value);


bool
font_set_metric_texture_id(const Font_data *data, const uint32_t key, const uint32_t *set_value);




} // Data ns


#endif // inc guard
