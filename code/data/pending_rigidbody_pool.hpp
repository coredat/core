#ifndef PENDING_RIGIDBODY_POOL_INCLUDED_152796BB_AB1E_4365_A0D3_C71F6F797041
#define PENDING_RIGIDBODY_POOL_INCLUDED_152796BB_AB1E_4365_A0D3_C71F6F797041


#include <systems/entity/entity_id.hpp>
#include <systems/physics/physics_fwd.hpp>
#include <systems/physics/rigidbody_properties.hpp>


#define PENDING_RIGIDBODY_POOL_SIZE 128


namespace Data {


struct Pending_rigidbody_pool
{
  Physics::Rigidbody*             rigidbody_out[PENDING_RIGIDBODY_POOL_SIZE];
  Physics::Rigidbody_properties   rigidbody_property[PENDING_RIGIDBODY_POOL_SIZE];
  
  std::size_t size = 0;
  const std::size_t capacity = PENDING_RIGIDBODY_POOL_SIZE;
}; // struct


/*!
  Inits the pending rb pool
*/
void
pending_rigidbody_pool_init(Pending_rigidbody_pool *pool);


/*!
  Push new pending rb onto the stack
*/
void
pending_rigidbody_pool_push(Pending_rigidbody_pool *pool, const Physics::Rigidbody_properties props, Physics::Rigidbody *output);


/*!
  Clear stack
*/
void
pending_rigidbody_pool_clear(Pending_rigidbody_pool *pool);

} // ns


#endif // inc guard