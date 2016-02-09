#ifndef ENTITY_POOL_INCLUDED_1A43BE4E_FC2B_4052_98FB_C7D78CD94C07
#define ENTITY_POOL_INCLUDED_1A43BE4E_FC2B_4052_98FB_C7D78CD94C07


#include <application/resources.hpp>
#include <core/entity_id.hpp>
#include <math/math.hpp>
#include <simple_renderer/lazy_include.hpp>
#include <systems/physics/physics_fwd.hpp>
#include <core/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <stddef.h>


#define ENTITY_POOL_SIZE 128


namespace Data {


/*!
  Generic entity properties.
*/
struct Entity_properties
{
  uint32_t tags;
};


/*!
  Generic entity storeage. aka the scene graph.
*/
struct Entity_pool
{
  Core::Entity_id                 entity_id[ENTITY_POOL_SIZE];
  Core::Entity_id                 parent_id[ENTITY_POOL_SIZE];
  Entity_properties               entity_properties[ENTITY_POOL_SIZE];
  
  math::transform                 transform[ENTITY_POOL_SIZE];
  
  bool                            display[ENTITY_POOL_SIZE];
  
  Resource::Model::ENUM           model[ENTITY_POOL_SIZE];
  Resource::Texture::ENUM         texture[ENTITY_POOL_SIZE];
  
  Physics::Rigidbody_properties   rigidbody_property[ENTITY_POOL_SIZE];
  Physics::Rigidbody_collider     rigidbody_collider[ENTITY_POOL_SIZE];
  
  const size_t                    size = ENTITY_POOL_SIZE;
};



/*!
  Initializes the pool to all zeros.
  \param pool The pool you wish to init.
*/
void
entity_pool_init(Entity_pool *pool);


/*!
  De intiialize the pool.
  \param The entity pool.
*/
void
entity_pool_de_init(Entity_pool *pool);



/*!
 Push a new entity into the pool.
 \param pool entity_pool.
 \param id The entity id that you wish to insert.
 \return true if it successfully inserted an object.
*/
bool
entity_pool_push_new_entity(Entity_pool *pool, const Core::Entity_id id);


/*!
  Remove an entity fromt the pool.
  \param pool the entity_pool.
  \param id The entity_id you wish to remove.
  \return true if the id was found and removed.
*/
bool
entity_pool_remove_entity(Entity_pool *pool, const Core::Entity_id id);


} // ns


#endif // inc guard