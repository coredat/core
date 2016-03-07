#include "level_functions.hpp"


namespace Level {


math::vec3
get_point_on_cirlce(const float a)
{
  const float radius = 2.f;
  const float origin_x = 0.f;
  const float origin_y = 0.f;
  
  const float x = origin_x + radius * math::cos(a);
  const float y = origin_y + radius * math::sin(a);
  constexpr float z = 0.f;
  
  return math::vec3_init(x, y, z);
}


} // ns