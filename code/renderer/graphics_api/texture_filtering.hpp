#ifndef TEXTURE_FILTER_INCLUDED_52EA5AEA_AF9A_48D3_B808_4FBD5B032C56
#define TEXTURE_FILTER_INCLUDED_52EA5AEA_AF9A_48D3_B808_4FBD5B032C56


#include <stdint.h>


namespace Graphics_api {


/*!
  Texture Options
*/
struct Texture_filtering
{
  uint8_t wrap_s : 2;
  uint8_t wrap_t : 2;
  uint8_t filtering : 4;
};


namespace Filtering_mode {

enum ENUM {

  point       = 1 << 0,
  bilinear    = 1 << 1,
  trilinear   = 1 << 2,

}; // enum

} // ns


namespace Wrap_mode {

enum ENUM {

  clamp     = 1 << 0,
  wrap      = 1 << 1,

}; // enum

} // ns


} // ns


#endif // inc guard