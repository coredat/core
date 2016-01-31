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
  Generic entity storeage. aka the scene graph.
*/
struct Entity_pool
{
  Core::Entity_id                 entity_id[ENTITY_POOL_SIZE];
  Core::Entity_id                 parent_id[ENTITY_POOL_SIZE];
  
  math::transform                 transform[ENTITY_POOL_SIZE];
  
  bool                            display[ENTITY_POOL_SIZE];
  
  Resource::Model::ENUM           model[ENTITY_POOL_SIZE];
  Resource::Texture::ENUM         texture[ENTITY_POOL_SIZE];
  
  Physics::Rigidbody_properties   rigidbody_property[ENTITY_POOL_SIZE];
  Physics::Rigidbody_collider     rigidbody_collider[ENTITY_POOL_SIZE];
  
  const size_t                    size = ENTITY_POOL_SIZE;
};


/*!
  Entities that have 'changed' in the tree in the last frame.
*/

enum class Entity_graph_change
{
  inserted,
  moved,
  updated,
  removed,
};


struct Entity_change
{
  Entity_graph_change change_type;
  Core::Entity_id entity_id;
};


struct Entity_graph_changes_pool
{
  Entity_change       entity_event[ENTITY_POOL_SIZE];
  size_t              size;
  const size_t        capacity = ENTITY_POOL_SIZE;
};


/*!
  Initializes the pool to all zeros.
*/
void
entity_pool_init(Entity_pool *pool);


/*!
  Graph changes events
*/
void
entity_graph_change_pool_init(Entity_graph_changes_pool *pool);


/*!
  Push pending graph changes in
*/
void
entity_graph_change_push(Entity_graph_changes_pool *pool, const Core::Entity_id entity_id, Entity_graph_change change_type);


} // ns


#endif // inc guard