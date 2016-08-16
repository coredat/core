#include <core/camera/camera_utils.hpp>
#include <core/camera/camera.hpp>
#include <core/camera/camera_properties.hpp>
#include <core/context/context.hpp>
#include <core/context/world.hpp>
#include <core/physics/ray.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/transform/transform.hpp>
#include <math/mat/mat4.hpp>
#include <assert.h>


namespace Core {
namespace Camera_utils {


math::mat4
camera_get_projection_matrix(const Camera &camera)
{
  if(camera.get_type() == Core::Camera_type::perspective)
  {
    return math::mat4_projection(camera.get_width(),
                                 camera.get_height(),
                                 camera.get_near_plane(),
                                 camera.get_far_plane(),
                                 camera.get_field_of_view());
  }
  else
  {
    return math::mat4_orthographic(camera.get_width(),
                                   camera.get_height(),
                                   camera.get_near_plane(),
                                   camera.get_far_plane());
  }
}


math::mat4
camera_get_view_matrix(const Camera &camera)
{
  const Entity_ref attached_entity = camera.get_attached_entity();
  
  assert(attached_entity.is_valid());
  
  const Transform trans = attached_entity.get_transform();
  
  return math::mat4_lookat(trans.get_position(),
                           math::vec3_add(trans.get_position(), trans.get_forward()),
                           trans.get_up());
}



Ray
unproject(const Camera &camera, const Context &ctx, World &world)
{
  return Ray(world, math::vec3_zero(), math::vec3_one());
}


} // ns
} // ns