#ifndef CU3E_MATH_EXTENSIONS_INCLUDED_95DF233B_686A_4DBA_A812_523BC62E1BFE
#define CU3E_MATH_EXTENSIONS_INCLUDED_95DF233B_686A_4DBA_A812_523BC62E1BFE


#include <math/transform/transform.hpp>


typedef float r32;
struct q3Vec3;
struct q3Quaternion;
struct q3Transform;


namespace math {


q3Vec3
vec3_to_q3vec(const math::vec3 vec);


vec3
vec3_from_q3vec(const q3Vec3 vec);


q3Quaternion
quat_to_q3quat(const math::quat quat);


math::transform
transform_init_from_q3(const q3Transform transform);


void
transform_to_q3(const math::transform *math_transform,
                r32 *out_angle = nullptr,
                q3Vec3 *out_axis = nullptr,
                q3Vec3 *out_position = nullptr);


} // ns


#endif // inc guard