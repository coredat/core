#ifndef PHYSICS_INCLUDED_1D1EF33A_9D5D_483E_853F_E7640773D08B
#define PHYSICS_INCLUDED_1D1EF33A_9D5D_483E_853F_E7640773D08B


#include <core/common/core_fwd.hpp>


namespace Core {
namespace Physics {


void
apply_force(Core::Entity_ref ref,
            const math::vec3 world_direction,
            const float force);


} // ns
} // ns


#endif // inc guard