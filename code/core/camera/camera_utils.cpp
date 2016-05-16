#include <core/camera/camera_utils.hpp>
#include <core/camera/camera.hpp>
#include <math/mat/mat4.hpp>


namespace Core {
namespace Camera_utils {


math::mat4
camera_get_projection_matrix(const Camera &camera)
{
  return math::mat4_projection(camera.get_width(),
                               camera.get_height(),
                               camera.get_near_plane(),
                               camera.get_far_plane(),
                               camera.get_field_of_view());
}


math::mat4
camera_get_inverse_projection_matrix(const Camera &camera)
{
  return math::mat4_get_inverse(camera_get_projection_matrix(camera));
}


} // ns
} // ns