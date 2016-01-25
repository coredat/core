#ifndef RIGIDBODY_POOL_INCLUDED_A8688A7A_C876_4FAA_85DE_D4D644C57B53
#define RIGIDBODY_POOL_INCLUDED_A8688A7A_C876_4FAA_85DE_D4D644C57B53


#include <systems/entity/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <systems/physics/rigidbody_properties.hpp>
#include <systems/physics/rigidbody_collider.hpp>
#include <stddef.h>


#define RIGIDBODY_POOL_SIZE 512


namespace Data {


/*!
  Holds the rigidbodies active in the scene.
  We only need to hold on to these for cleanup,
  as bullet will deal with everything else.
*/
struct Rigidbody_pool
{
  ::Entity::Entity_id               entity_id[RIGIDBODY_POOL_SIZE];
  Physics::Rigidbody                rigidbody[RIGIDBODY_POOL_SIZE];
  
  const size_t                      size{RIGIDBODY_POOL_SIZE};
}; // ns



/*!
  Update structure holds the things we need
  to update a rigidbody.
*/
struct Rigidbody_update
{
  ::Entity::Entity_id             parent_id;
  Physics::Rigidbody_collider     collider_info;
  Physics::Rigidbody_properties   properties;
};


/*!
  Rigidbodies that need to be updated.
  this is a temp storage place. The engine will
  pick up these at a later date.
*/
struct Rigidbody_update_pool
{
  ::Entity::Entity_id entity_id[RIGIDBODY_POOL_SIZE];
  Rigidbody_update    rb_updates[RIGIDBODY_POOL_SIZE];
  
  size_t              size = {0};
  const size_t        capacity = RIGIDBODY_POOL_SIZE;
};


/*!
 Initialises the entire rb pool.
*/
void
rigidbody_pool_init(Rigidbody_pool *pool);


/*!
 Initialises the entire pending rb pool.
*/
void
rigidbody_update_pool_init(Rigidbody_update_pool *pool);


/*!
  Clears out the data that has been used.
*/
void
rigidbody_update_pool_clear(Rigidbody_update_pool *pool);


/*!
  Pushes a new entity into the update pool so it can be
  created later.
 
  \return true of false if it was a success or failure.
*/
bool
rigidbody_update_pool_add_update(Rigidbody_update_pool *update_pool,
                                 const ::Entity::Entity_id id,
                                 const ::Entity::Entity_id parent_id,
                                 const Physics::Rigidbody_collider collider,
                                 const Physics::Rigidbody_properties props);

/*!
  Process thes pending updates.
*/
void
rigidbody_pool_process_updates(Physics::World *phy_world,
                               Data::World *data,
                               Rigidbody_update_pool *update_pool,
                               Rigidbody_pool *rb_pool);



} // ns


#endif // inc guard