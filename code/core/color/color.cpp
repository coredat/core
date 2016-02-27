#include "color.hpp"


namespace
{
  constexpr float
  to_float(const uint32_t color, const uint32_t offset)
  {
    return static_cast<float>((color >> offset) & 0xFF) / 255.f;
  }
}



namespace Core {


float
Color::get_red() const
{
  return to_float(m_color, 24);
}


float
Color::get_green() const
{
  return to_float(m_color, 16);
}


float
Color::get_blue() const
{
  return to_float(m_color, 8);
}


float
Color::get_alpha() const
{
  return to_float(m_color, 0);
}


} // ns