#ifndef CAMERA_UTIL_INCLUDED_816C1012_995F_4633_A19A_8B8C44D69E79
#define CAMERA_UTIL_INCLUDED_816C1012_995F_4633_A19A_8B8C44D69E79


#include <core/common/core_fwd.hpp>
#include <core/physics/ray_search.hpp>
#include <math/mat/mat_types.hpp>


namespace Core {
namespace Camera_utils {


/*!
  Builds a projection matrix from the camera properties.
*/
math::mat4
get_projection_matrix(const Camera &camera);


/*!
  Builds a view matrix from the camera properties.
*/
math::mat4
get_view_matrix(const Camera &camera);


/*!
  Get a ray cast from a camera.
*/
Ray
get_ray_from_viewport(const Camera &camera,
                      const Axis viewport_coords);


/*!
  Gets the world position on the nearplane from a screen coord.
*/
math::vec3
get_world_position_on_nearplane(const Camera &camera,
                                const Axis viewport_coords);


/*!
  Get a plane that represents the near plane.
  The normal is facing the camera.
*/
Plane
get_near_plane(const Camera &camera);


/*!
  Get a plane that represents the far plane.
  The normal is facing the camera.
*/
Plane
get_near_plane(const Camera &camera);


} // ns
} // ns


#endif // inc guard