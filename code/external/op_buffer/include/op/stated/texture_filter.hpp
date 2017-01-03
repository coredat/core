#ifndef TEXTURE_FILTER_INCLUDED_9039AA1C_7C81_4352_8F5A_F7FBDB446E92
#define TEXTURE_FILTER_INCLUDED_9039AA1C_7C81_4352_8F5A_F7FBDB446E92


namespace op {
namespace texture_wrap_mode {


enum ENUM : uint8_t
{
  CLAMP,
  REPEAT,
};


} // ns


namespace texture_filter_mode {


enum ENUM : uint8_t
{
  POINT,
  BILINEAR,
  TRILINEAR,    // Falls back to Bilinear if no mips.
  ANISOTROPIC,  // Falls back to Bilinear if no mips.
};


} // ns
} // ns


#endif // inc guard
