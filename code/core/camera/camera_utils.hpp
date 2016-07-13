#ifndef CAMERA_UTIL_INCLUDED_816C1012_995F_4633_A19A_8B8C44D69E79
#define CAMERA_UTIL_INCLUDED_816C1012_995F_4633_A19A_8B8C44D69E79


#include <core/camera/camera_fwd.hpp>
#include <math/mat/mat_types.hpp>


/*
  Experimental.
*/


namespace Core {
namespace Camera_utils {


math::mat4
camera_get_projection_matrix(const Camera &camera);


math::mat4
camera_get_inverse_projection_matrix(const Camera &camera);


math::mat4
camera_get_view_matrix(const Camera &camera);


math::mat4
camera_get_inverse_view_matrix(const Camera &cam);


} // ns
} // ns


#endif // inc guard