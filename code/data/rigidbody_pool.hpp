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
  
  const size_t                      size{RIGIDBODY_POOL_SIZE};
}; // ns


struct Rigidbody_update
{
  ::Entity::Entity_id             parent_id;
  Physics::Rigidbody_collider     collider_info;
  Physics::Rigidbody_properties   properties;
};


struct Rigidbody_update_pool
{
  ::Entity::Entity_id entity_id[RIGIDBODY_POOL_SIZE];
  Rigidbody_update    rb_updates[RIGIDBODY_POOL_SIZE];
  
  size_t              size = {0};
  const size_t        capacity = RIGIDBODY_POOL_SIZE;
};


void
rigidbody_pool_init(Rigidbody_pool *pool);


void
rigidbody_update_pool_init(Rigidbody_update_pool *pool);


void
rigidbody_update_pool_add_update(Rigidbody_update_pool *update_pool,
                                 const ::Entity::Entity_id id,
                                 const Physics::Rigidbody_collider collider,
                                 const Physics::Rigidbody_properties props);
                                 
void
rigidbody_pool_process_updates(Physics::World *phy_world, Data::World *data, Rigidbody_update_pool *update_pool, Rigidbody_pool *rb_pool);


void
rigidbody_pool_update_rb(Rigidbody_pool *pool,
                         const ::Entity::Entity_id id,
                         Physics::World *world,
                         Data::World *data,                         
                         const Physics::Rigidbody_properties &props,
                         const Physics::Rigidbody_collider &collider);

} // ns


#endif // inc guard