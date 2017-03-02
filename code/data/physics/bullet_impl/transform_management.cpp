#include "../physics.hpp"
#include "physics_data.hpp"
#include <utilities/utilities.hpp>


namespace Data {
namespace Physics {


void
transform_set(
  Physics_data *phys,
  const uint32_t id,
  const math::transform *transform)
{
  // -- Param Check -- //
  LIB_ASSERT(phys);
  LIB_ASSERT(id);
  LIB_ASSERT(transform);
  
  // -- Find transform and call update on it -- //
  if(lib::key::linear_search(
    id,
    phys->rb_ids.data(),
    phys->rb_ids.size()))
  {
    rigidbody_update(phys, id, nullptr, nullptr, transform, nullptr);
    return;
  }
  
  if(lib::key::linear_search(
    id,
    phys->trigger_ids.data(),
    phys->trigger_ids.size()))
  {
    trigger_udpate(phys, id, nullptr, nullptr, transform, nullptr);
    return;
  }
}


void
transform_callback_id(Physics_data *phys, const uint32_t callback_id)
{
  // -- Param Check -- //
  LIB_ASSERT(phys);
  LIB_ASSERT(callback_id);
  
  phys->transform_callback_id = callback_id;
}


} // ns
} // ns
