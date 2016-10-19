
/*
  Warning
  --
  This file is auto generated changes here may be overwritten.
*/


#ifndef FONT_GLYPH_INCLUDED_9D975A32_2F75_4068_B428_A15F5FB007BD
#define FONT_GLYPH_INCLUDED_9D975A32_2F75_4068_B428_A15F5FB007BD


#include <stdint.h>
#include <stddef.h>
#include <systems/text/character.hpp>


namespace Data {


/*!
  Font_glyph Model
*/
struct Font_glyph_data
{
  // key //
  uint32_t *keys = nullptr;

  // character field data //
  Text::Character *field_character = nullptr;

  // size //
  size_t size = 0;
  const size_t capacity = 0;
}; // Font_glyph Model


// ====================== //
// COMMON MODEL FUNCTIONS //
// ====================== //
void
data_lock(const Font_glyph_data *data);


void
data_unlock(const Font_glyph_data *data);


bool
font_glyph_create(Font_glyph_data *data, const size_t size_hint);


void
font_glyph_destroy(Font_glyph_data *data);


uint32_t
font_glyph_push(Font_glyph_data *data, const uint32_t key);


bool
font_glyph_insert(Font_glyph_data *data, const uint32_t key, const size_t insert_point);


bool
font_glyph_remove(Font_glyph_data *data, const uint32_t key);


bool
font_glyph_exists(const Font_glyph_data *data, const uint32_t key, size_t *out_index = nullptr);


void
font_glyph_clear(Font_glyph_data *data);


bool
font_glyph_is_empty(const Font_glyph_data *data);


size_t
font_glyph_get_size(const Font_glyph_data *data);


size_t
font_glyph_get_capacity(const Font_glyph_data *data);


bool
font_glyph_resize_capacity(Font_glyph_data *data, const size_t size_hint);



// ===================== //
// DATA GETTER FUNCTIONS //
// ===================== //


const Text::Character*
font_glyph_get_const_character_data(const Font_glyph_data *data);


Text::Character*
font_glyph_get_character_data(Font_glyph_data *data);


// =============== //
// FIELD FUNCTIONS //
// =============== //


bool
font_glyph_get_character(const Font_glyph_data *data, const uint32_t key, Text::Character *return_value);


bool
font_glyph_set_character(const Font_glyph_data *data, const uint32_t key, const Text::Character *set_value);




} // Data ns


#endif // inc guard
