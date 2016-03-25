#ifndef COLOR_INCLUDED_FA0621D5_83C0_4765_8B7C_E44F07C3EC2F
#define COLOR_INCLUDED_FA0621D5_83C0_4765_8B7C_E44F07C3EC2F


#include <stdint.h>


namespace Core {


/*!
  Represents a generic color.
  This is an immutable object.
  
  We represent color as.
  0xRRGGBBAA
*/
class Color
{
public:


  explicit constexpr
  Color(const uint32_t hex_value = 0xFFFFFFFF)
  : m_color(hex_value)
  {
  }
  
  
  explicit constexpr
  Color(const float r, const float g, const float b, const float a = 1.f)
  : m_color((uint32_t)((static_cast<uint32_t>(r * 255) << 24) |
                       (static_cast<uint32_t>(g * 255) << 16) |
                       (static_cast<uint32_t>(b * 255) << 8) |
                       (static_cast<uint32_t>(a * 255) << 0)))
  {
  }
  
  
  uint32_t          get_color() const { return m_color; }
  
  
private:

  const uint32_t        m_color;

};


} // ns


#endif // inc guard