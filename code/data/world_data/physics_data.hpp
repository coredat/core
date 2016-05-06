#ifndef PHYSICS_DATA_INCLUDED_2B43434B_691E_41EA_B754_04FCF853C466
#define PHYSICS_DATA_INCLUDED_2B43434B_691E_41EA_B754_04FCF853C466


#include <utilities/generic_id.hpp>
#include <core/physics/collider.hpp>
#include <math/geometry/geometry_types.hpp>
#include <math/transform/transform_types.hpp>
#include <stdint.h>


namespace World_data {


/*!
  The entities physics simulation.
*/
struct Physics_data
{
  util::generic_id    *entity_id      = nullptr;
  math::transform     *transform      = nullptr;
  math::aabb          *aabb_collider  = nullptr;
  
  uint32_t            size      = 0;
  uint32_t            capacity  = 0;
};


/*!
  Sets up the entity pool
  \param data The physics data.
  \param size How big the physics data should be.
*/
void
physics_init(Physics_data *data,
             const uint32_t size);


/*!
  Frees the entity pool.
  \param data The data to free.
*/
void
physics_free(Physics_data *data);


/*!
  Add an entity to The physics simulation data.
  \param data The data to free.
  \param entity_id Is the id of the entity.
  \param aabb The current aabb of the entity.
*/
void
physics_add(Physics_data *data,
            const util::generic_id entity_id,
            const math::aabb *aabb,
            const math::transform *trans);


/*!
  Update an entity in the simulation.
  \param data The Physics simulation data.
  \param entity_id The entity which needs updating.
  \param trans The entity transform.
*/
void
physics_update(Physics_data *data,
               const util::generic_id entity_id,
               const math::aabb *aabb,
               const math::transform *trans);


/*!
  Pull an entity out of the physics simulation.
  \param data The Phyiscs simulation data.
  \param entity_id The entity to remove.
*/
void
physics_remove(Physics_data *data,
               const util::generic_id entity_id);


} // ns


#endif // inc gaurd