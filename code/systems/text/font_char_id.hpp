#ifndef FONT_CHAR_ID_INCLUDED_D1286ADB_89F3_4C4D_91D6_C0DB64CCB300
#define FONT_CHAR_ID_INCLUDED_D1286ADB_89F3_4C4D_91D6_C0DB64CCB300


#include <stdint.h>


namespace Text {


struct Font_char_id
{
  uint64_t font_id : 8;
  uint64_t size_id : 8;
  uint64_t char_id : 48;
};


} // ns


#endif // inc guard