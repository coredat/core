#ifndef CAMERA_UTIL_INCLUDED_816C1012_995F_4633_A19A_8B8C44D69E79
#define CAMERA_UTIL_INCLUDED_816C1012_995F_4633_A19A_8B8C44D69E79


#include <core/common/core_fwd.hpp>
#include <math/mat/mat_types.hpp>


/*
  Experimental.
*/


namespace Core {
namespace Camera_utils {


math::mat4
get_projection_matrix(const Camera &camera);


math::mat4
get_view_matrix(const Camera &camera);


Ray
unproject(const Camera &camera, const Context &ctx, World &world);


} // ns
} // ns


#endif // inc guard