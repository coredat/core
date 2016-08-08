#ifndef FONT_INCLUDED_11F68AEA_3C63_45A8_ACDC_E5C05604C967
#define FONT_INCLUDED_11F68AEA_3C63_45A8_ACDC_E5C05604C967


#include <stdint.h>


namespace Text {


struct Font_bitmap
{
  uint32_t bitmap_offset[2]; // These are the bitmap offsets into.
  uint32_t bitmap_size[2]; // The size of the bitmap.
  uint32_t bitmap_channels; // The number of channels a bitmap has;
  uint32_t line_height;
};


} // ns


#endif // inc guard