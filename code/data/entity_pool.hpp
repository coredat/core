#ifndef ENTITY_POOL_INCLUDED_1A43BE4E_FC2B_4052_98FB_C7D78CD94C07
#define ENTITY_POOL_INCLUDED_1A43BE4E_FC2B_4052_98FB_C7D78CD94C07


#include <application/resources.hpp>
#include <systems/entity/entity_id.hpp>
#include <math/math.hpp>
#include <simple_renderer/lazy_include.hpp>
#include <systems/physics/physics_fwd.hpp>
#include <systems/entity/entity_id.hpp>
#include <systems/physics/physics.hpp>
#include <stddef.h>


#define ENTITY_POOL_SIZE 128


namespace Data {


struct Entity_pool
{
  Entity::Entity_id               entity_id[ENTITY_POOL_SIZE];
  Entity::Entity_id               parent_id[ENTITY_POOL_SIZE];
  
  math::transform                 transform[ENTITY_POOL_SIZE];
  
  bool                            display[ENTITY_POOL_SIZE];
  
  Resource::Model::ENUM           model[ENTITY_POOL_SIZE];
  Resource::Texture::ENUM         texture[ENTITY_POOL_SIZE];
  
  Physics::Rigidbody              rigidbody[ENTITY_POOL_SIZE]; // KILL
  Physics::Rigidbody_properties   rigidbody_property[ENTITY_POOL_SIZE];
  Physics::Rigidbody_collider     rigidbody_collider[ENTITY_POOL_SIZE];
  
  const size_t                    size = ENTITY_POOL_SIZE;
};


/*!
  Initializes the pool to all zeros
*/
void
entity_pool_init(Entity_pool *pool);


} // ns


#endif // inc guard