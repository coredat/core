#ifndef TRANSFORM_INCLUDED_E79342EA_D69A_4FEB_BEAD_BA21D7B8B8E6
#define TRANSFORM_INCLUDED_E79342EA_D69A_4FEB_BEAD_BA21D7B8B8E6


#include "../transform/transform_types.hpp"
#include "../quaternion/quaternion.hpp"
#include "../vector/vector3.hpp"
#include "../matrix/matrix44.hpp"


namespace caffeine {
namespace math {


inline matrix44   get_world_matrix(const transform &transform);
inline void       set_with_world_matrix(transform &transform, const matrix44 &matrix);


} // namespace
} // namespace


#endif // include guard