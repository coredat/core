#include <core/camera/camera_utils.hpp>
#include <core/camera/camera.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/transform/transform.hpp>
#include <math/mat/mat4.hpp>
#include <assert.h>


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


math::mat4
camera_get_view_matrix(const Camera &camera)
{
  const Entity_ref attached_entity = camera.get_attached_entity();
  
  assert(attached_entity.is_valid());
  
  const Transform trans = attached_entity.get_transform();
  
  return math::mat4_lookat(trans.get_position(), trans.get_forward(), trans.get_up());
}


math::mat4
camera_get_inverse_view_matrix(const Camera &cam)
{
  return math::mat4_get_inverse(camera_get_view_matrix(cam));
}


} // ns
} // ns