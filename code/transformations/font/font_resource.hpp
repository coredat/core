#ifndef FONT_RESOURCE_INCLUDED_8FA65EE8_C3F1_4B79_9B82_32147C8099EC
#define FONT_RESOURCE_INCLUDED_8FA65EE8_C3F1_4B79_9B82_32147C8099EC


#include <data/fwd_dec_data.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Font_resource {


uint32_t
add_new_font(const char *filename,
             Data::Font_data *font_data,
             Data::Texture_data *texture_data);
  
  
void
add_glyphs(const char *glyph_arr,
           const size_t glyph_arr_count,
           const uint32_t font_id,
           Data::Font_data *font_data,
           Data::Font_glyph_data *glyph_data,
           Data::Texture_data *texture_data);


} // ns


#endif // inc guard