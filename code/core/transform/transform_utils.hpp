#ifndef TRANSFORM_UTILS_INCLUDED_92F304D0_0F0A_410C_96ED_0F82A1775053
#define TRANSFORM_UTILS_INCLUDED_92F304D0_0F0A_410C_96ED_0F82A1775053


#include <core/common/core_fwd.hpp>
#include <math/mat/mat_fwd.hpp>
#include <math/vec/vec_fwd.hpp>


namespace Core {
namespace Transform_utils {


math::vec3
get_world_forward();


math::vec3
get_world_up();


math::vec3
get_world_left();


math::mat4
get_world_matrix(const Core::Transform &transform);


} // ns
} // ns


#endif // inc guard