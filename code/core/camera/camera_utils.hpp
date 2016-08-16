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
  Casts a ray from the viewport into the the world.
*/
Ray
viewport_to_ray(const Camera &camera,
                const World &world,
                const Axis viewport_coords,
                const Axis viewport_size,
                const Ray_search search = Ray_search::first);


} // ns
} // ns


#endif // inc guard