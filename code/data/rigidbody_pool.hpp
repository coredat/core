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
  Core::Entity_id               entity_id[RIGIDBODY_POOL_SIZE];
  Physics::Rigidbody                rigidbody[RIGIDBODY_POOL_SIZE];
  
  const size_t                      size{RIGIDBODY_POOL_SIZE};
}; // ns


/*!
 Initialises the entire rb pool.
*/
void
rigidbody_pool_init(Rigidbody_pool *pool);


/*!
  Find an rb inside the pool.
  \param pool of rbs.
  \param id is the entity_id you wish to find.
  \param out_rb is the entity that was found.
  \return true on success.
*/
bool
rigidbody_pool_find(Rigidbody_pool *pool,
                    const Core::Entity_id id,
                    Physics::Rigidbody **out_rb);

/*!
  Removes an entity from the pool.
  Does not remove it from the physics simulation.
  Its id will be set to invalid_id.
  \param pool or rbs.
  \param id you wish to remove.
  \return true if we found an id to remove.
*/
bool
rigidbody_pool_remove(Rigidbody_pool *pool,
                      const Core::Entity_id id);


/*!
  Checks to see if an entity exists in the pool.
  \param pool or rbs.
  \param id you wish to remove.
  \return true if we found an id to remove.
*/
bool
rigidbody_pool_exists(Rigidbody_pool *pool,
                      const Core::Entity_id id);


/*!
  Allocate a new place for the entity.
  Optinally you may add an rigidbody to get the result position.
  \param pool of rbs
  \param id of where we wish to push the new data.
  \param new_rb the new rb we wish to push.
  \return true if we found the requeseted index and pushed new data.
*/
bool
rigidbody_pool_push(Rigidbody_pool *pool,
                    const Core::Entity_id id,
                    Physics::Rigidbody **new_rb);


/*!
  We want to know about updates to the scene graph.
*/
void
rigidbody_pool_update_scene_graph_changes(Rigidbody_pool *pool,
                                          Data::World *world_data,
                                          const Entity_graph_changes_pool *graph_changes);


} // ns

#endif // inc guard