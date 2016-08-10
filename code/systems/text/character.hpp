#ifndef CHARACTER_INCLUDED_22326842_0286_4E4F_AD40_FA10F084A6C4
#define CHARACTER_INCLUDED_22326842_0286_4E4F_AD40_FA10F084A6C4


#include <stdint.h>


namespace Text {


struct Character
{
  float uv[2];
  float st[2];
  float size[2];
  float advance[2];
  float offset[2];
};


} // ns


#endif // inc guard