#ifndef FONT_DATA_INCLUDED_089E3839_0967_48F1_B9EE_DEFB451F1238
#define FONT_DATA_INCLUDED_089E3839_0967_48F1_B9EE_DEFB451F1238


#include <op/op.hpp>
#include <stdint.h>
#include <3rdparty/stb/stb_truetype.h>


namespace Data {


struct Character
{
  float uv[2];
  float st[2];
  float size[2];
  float advance[2];
  float offset[2];
  
  float padding[2];
};


struct Font_bitmap
{
  uint32_t bitmap_offset[2]; // These are the bitmap offsets into.
  uint32_t bitmap_size[2]; // The size of the bitmap.
  uint32_t bitmap_channels; // The number of channels a bitmap has;
  uint32_t line_height;
  float    scale;
  float    decsent;
  float    ascent;
};


struct Font
{
  char           name[256];
  Font_bitmap    bitmap_info;
  stbtt_fontinfo font_info;
  opID           glyph_texture_id;
  opID           metrics_texture_id;
};


} // ns


#endif // inc guard