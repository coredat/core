#ifndef PHYSICS_DATA_INCLUDED_2B43434B_691E_41EA_B754_04FCF853C466
#define PHYSICS_DATA_INCLUDED_2B43434B_691E_41EA_B754_04FCF853C466


#include <utilities/generic_id.hpp>
#include <core/physics/collider.hpp>
#include <math/geometry/geometry_types.hpp>
#include <math/transform/transform_types.hpp>
#include <stdint.h>


namespace World_data {


struct Physics_data
{
  util::generic_id     *entity_id      = nullptr;
  math::transform     *transform      = nullptr;
  math::aabb          *aabb_collider  = nullptr;
  
  uint32_t            size      = 0;
  uint32_t            capacity  = 0;
};


void
physics_init(Physics_data *data, const uint32_t size);


void
physics_free(Physics_data *data);


void
physics_add(Physics_data *data,
            const util::generic_id id,
            const math::aabb *aabb,
            const math::transform *trans);


void
physics_remove(Physics_data *data,
               const util::generic_id id);


void
physics_update(Physics_data *data,
               const util::generic_id id,
               const math::aabb *aabb,
               const math::transform *trans);

uint32_t
physics_find_index(Physics_data *data, const util::generic_id id);


} // ns


#endif // inc gaurd