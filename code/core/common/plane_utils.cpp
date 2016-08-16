#include <core/common/plane_utils.hpp>
#include <core/common/plane.hpp>
#include <core/common/ray.hpp>
#include <math/vec/vec3.hpp>


namespace Core {
namespace Plane_utils {


bool
ray_intersects_with_plane(const Plane &plane, const Ray &ray, float &out_distance)
{
  constexpr float error = 0.000001f;

  const float dot = math::vec3_dot(plane.get_normal(), ray.get_direction());
  
  if (math::abs(dot) > error)
  {
    const math::vec3 distance = math::vec3_subtract(plane.get_position(), ray.get_origin());
    const float dot_norm = math::vec3_dot(distance, plane.get_normal()) / dot;
  
    out_distance = dot_norm;
    
    if(out_distance >= 0)
    {
      return true;
    }
  }
  
  return false;
}


} // ns
} // ns