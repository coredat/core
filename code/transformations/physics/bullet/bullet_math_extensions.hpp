#ifndef BULLET_MATH_EXTENSIONS_INCLUDED_35E8A9CD_0A4F_4C37_98FB_1316D3D03B60
#define BULLET_MATH_EXTENSIONS_INCLUDED_35E8A9CD_0A4F_4C37_98FB_1316D3D03B60


#include <core/transform/transform_fwd.hpp>
#include <math/vec/vec_fwd.hpp>


class btVector3;
class btTransform;


namespace math {


math::vec3
vec3_from_bt(const btVector3 &bt_vec);


btVector3
vec3_to_bt(const math::vec3 vec);


btTransform
transform_to_bt(const Core::Transform &bt_trans);


Core::Transform
transform_from_bt(const btTransform &transform);


} // ns


#endif // inc guard