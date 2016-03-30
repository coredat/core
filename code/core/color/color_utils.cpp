#include <core/color/color_utils.hpp>


namespace
{
  constexpr uint8_t
  to_int(const uint32_t color, const uint32_t offset)
  {
    return (color >> offset) & 0xFF;
  }

  constexpr float
  to_float(const uint32_t color, const uint32_t offset)
  {
    return static_cast<float>(to_int(color, offset)) / 255.f;
  }
}


namespace Core {
namespace Color_utils {


float
get_red_f(const Color color)
{
  return to_float(color.get_color(), 24);
}


float
get_green_f(const Color color)
{
  return to_float(color.get_color(), 16);
}


float
get_blue_f(const Color color)
{
  return to_float(color.get_color(), 8);
}


float
get_alpha_f(const Color color)
{
  return to_float(color.get_color(), 0);
}


uint8_t
get_red_i(const Color color)
{
  return to_int(color.get_color(), 24);
}


uint8_t
get_green_i(const Color color)
{
  return to_int(color.get_color(), 16);
}


uint8_t
get_blue_i(const Color color)
{
  return to_int(color.get_color(), 8);
}


uint8_t
get_alpha_i(const Color color)
{
  return to_int(color.get_color(), 0);
}


} // ns
} // ns