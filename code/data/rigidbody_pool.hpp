#ifndef RIGIDBODY_POOL_INCLUDED_A8688A7A_C876_4FAA_85DE_D4D644C57B53
#define RIGIDBODY_POOL_INCLUDED_A8688A7A_C876_4FAA_85DE_D4D644C57B53


#include <systems/entity/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <stddef.h>


#define RIGIDBODY_POOL_SIZE 512


namespace Data {


struct Rigidbody_pool
{
  ::Entity::Entity_id       entity_id[RIGIDBODY_POOL_SIZE];
  Physics::Rigidbody        rigidbody[RIGIDBODY_POOL_SIZE];
  
  const size_t              size{RIGIDBODY_POOL_SIZE};
}; // ns



void
rigidbody_pool_init(Rigidbody_pool *pool);


} // ns


#endif // inc guard