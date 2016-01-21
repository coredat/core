#ifndef RIGIDBODY_POOL_INCLUDED_A8688A7A_C876_4FAA_85DE_D4D644C57B53
#define RIGIDBODY_POOL_INCLUDED_A8688A7A_C876_4FAA_85DE_D4D644C57B53


#include <systems/entity/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <systems/physics/rigidbody_properties.hpp>
#include <systems/physics/rigidbody_collider.hpp>
#include <stddef.h>


#define RIGIDBODY_POOL_SIZE 512


namespace Data {


struct Rigidbody_pool
{
  ::Entity::Entity_id               entity_id[RIGIDBODY_POOL_SIZE];
  Physics::Rigidbody                rigidbody[RIGIDBODY_POOL_SIZE];
  Physics::Rigidbody_properties     property[RIGIDBODY_POOL_SIZE];
  Physics::Rigidbody_collider       collider[RIGIDBODY_POOL_SIZE];
  
  const size_t                      size{RIGIDBODY_POOL_SIZE};
}; // ns



void
rigidbody_pool_init(Rigidbody_pool *pool);


void
rigidbody_pool_update_rb(Rigidbody_pool *pool,
                         const ::Entity::Entity_id id,
                         Physics::World *world,
                         Data::World *data,                         
                         const Physics::Rigidbody_properties &props,
                         const Physics::Rigidbody_collider &collider);


} // ns


#endif // inc guard