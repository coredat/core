#include <core/camera/camera_utils.hpp>
#include <core/camera/camera.hpp>
#include <core/input/axis.hpp>
#include <core/common/ray.hpp>
#include <core/common/plane.hpp>
#include <core/common/plane_utils.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/transform/transform.hpp>
#include <math/mat/mat4.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace Core {
namespace Camera_utils {


math::mat4
get_projection_matrix(const Camera &camera)
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
get_view_matrix(const Camera &camera)
{
  const Entity_ref attached_entity = camera.get_attached_entity();
  
  assert(attached_entity.is_valid());
  
  const Transform trans = attached_entity.get_transform();
  
  return math::mat4_lookat(trans.get_position(),
                           math::vec3_add(trans.get_position(), trans.get_forward()),
                           trans.get_up());
}


Ray
get_ray_from_viewport(const Camera &camera, const Axis viewport_coords)
{
  // Some Mouse things
  const Axis viewport_size {
    math::to_float(camera.get_width()),
    math::to_float(camera.get_height())
  };
  
  const Axis clamped_viewport {
    math::clamp(viewport_coords.x, 0, viewport_size.x),
    math::clamp(viewport_coords.y, 0, viewport_size.y)
  };

  if(camera.get_type() == Core::Camera_type::perspective)
  {
    const Core::Axis coords {
      (((2.f * clamped_viewport.x) / viewport_size.x) - 1.f),
      1.f - (2.f * clamped_viewport.y) / viewport_size.y
    };
  
    // --
    
    const math::vec3 ray_nds  = math::vec3_init(coords.x, coords.y, 1.f);
    const math::vec4 ray_clip = math::vec4_init(math::get_x(ray_nds),
                                                math::get_y(ray_nds),
                                                1.f,
                                                1.f);
    
    const math::mat4 proj_mat     = Core::Camera_utils::get_projection_matrix(camera);
    const math::mat4 proj_inv_mat = math::mat4_get_inverse(proj_mat);
    
    const math::vec4 ray_eye_get  = math::mat4_multiply(ray_clip, proj_inv_mat);
    const math::vec4 ray_eye      = math::vec4_init(math::get_x(ray_eye_get),
                                                    math::get_y(ray_eye_get),
                                                    -1.f,
                                                    0.f);
    
    // --
    
    const math::mat4 view_mat     = Core::Camera_utils::get_view_matrix(camera);
    const math::mat4 view_inv_mat = math::mat4_get_inverse(view_mat);
    
    const math::vec4 ray_wor_all = math::mat4_multiply(ray_eye, view_inv_mat);
    const math::vec3 ray_wor     = math::vec3_normalize(math::vec3_init(math::get_x(ray_wor_all),
                                                                        math::get_y(ray_wor_all),
                                                                        math::get_z(ray_wor_all)));

    // --

    const math::vec3 ray_start = camera.get_attached_entity().get_transform().get_position();
    const math::vec3 ray_dir   = ray_wor;
    
    return Core::Ray(ray_start, ray_dir);
  }
  else if(camera.get_type() == Core::Camera_type::orthographic)
  {
    const Core::Axis coords {
      ((viewport_size.x * 0.5f) - clamped_viewport.x) * -1.f,
      (viewport_size.y * 0.5f) - clamped_viewport.y
    };
    
    const Core::Transform cam_tran = camera.get_attached_entity().get_transform();
    
    const math::vec3 left = math::vec3_scale(cam_tran.get_left(), coords.x);
    const math::vec3 up   = math::vec3_scale(cam_tran.get_up(), coords.y);
    
    const math::vec3 cam_pos = cam_tran.get_position();
    
    const math::vec3 ray_start = math::vec3_add(math::vec3_add(cam_pos, left), up);
    const math::vec3 ray_dir   = cam_tran.get_forward();

    return Core::Ray(ray_start, ray_dir);
  }
  else
  {
    assert(false);
    LOG_FATAL("This is unreachable code");
    return Core::Ray(math::vec3_zero(), math::vec3_zero_zero_one());
  }
  
  // UNREACHABLE //
}
 

math::vec3
get_world_position_on_nearplane(const Camera &camera,
                                const Axis viewport_coords)
{
  // Create a plane that represents the near plane.
  const Core::Transform cam_tran = camera.get_attached_entity().get_transform();
  
  const math::vec3 scaled_fwd = math::vec3_scale(cam_tran.get_forward(), camera.get_near_plane());
  const math::vec3 offset_pos = math::vec3_add(cam_tran.get_position(), scaled_fwd);
  const math::vec3 plane_norm = math::vec3_scale(cam_tran.get_forward(), -1);
  
  const Core::Plane plane(offset_pos, plane_norm);
  const Core::Ray ray = get_ray_from_viewport(camera, viewport_coords);
  
  float distance = 0;
  const bool hit = Core::Plane_utils::ray_intersects_with_plane(plane, ray, distance);
  
  // This should always intersect as we know
  // the nearplane is in front of the camera.
  assert(hit);
  assert(distance > 0);
  
  if(camera.get_type() == Camera_type::perspective)
  {
    const math::vec3 scaled_dir = math::vec3_scale(ray.get_direction(), distance);
    return math::vec3_add(cam_tran.get_position(), scaled_dir);
  }
  
  else if(camera.get_type() == Camera_type::orthographic)
  {
    const math::vec3 scaled_fwd = math::vec3_scale(cam_tran.get_forward(), distance);
    return math::vec3_add(ray.get_origin(), scaled_fwd);
  }
  
  assert(false);
  LOG_FATAL("Not should be in the code path");
}


namespace {


/*
  Generic plane creation method.
  Takes a transform and a distance and creates a plane from it.
  The plane normal is the transform forward reversed.
*/
Core::Plane
create_plane(const Core::Transform &trans, const float distance)
{
  const math::vec3 cam_pos  = trans.get_position();
  const math::vec3 move_fwd = math::vec3_scale(trans.get_forward(), distance);
  const math::vec3 position = math::vec3_add(cam_pos, move_fwd);
  const math::vec3 normal   = math::vec3_scale(trans.get_forward(), -1.f);

  return Core::Plane(position, normal);
}


} // anon ns


Plane
get_near_plane(const Camera &camera)
{
  const Core::Transform trans = camera.get_attached_entity().get_transform();
  return create_plane(trans, camera.get_near_plane());
}


Plane
get_far_plane(const Camera &camera)
{
  const Core::Transform trans = camera.get_attached_entity().get_transform();
  return create_plane(trans, camera.get_far_plane());
}

} // ns
} // ns