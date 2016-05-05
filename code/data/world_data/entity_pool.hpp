#ifndef ENTITY_POOL_INCLUDED_1A43BE4E_FC2B_4052_98FB_C7D78CD94C07
#define ENTITY_POOL_INCLUDED_1A43BE4E_FC2B_4052_98FB_C7D78CD94C07


#include <utilities/generic_id.hpp>
#include <math/transform/transform_types.hpp>
#include <math/geometry/geometry_types.hpp>
#include <utilities/generic_id.hpp>
#include <stddef.h>


#define ENTITY_POOL_SIZE 2048


namespace World_data {


/*!
  Generic entity properties.
  Currently on contains tags, but other things like name could live here.
*/
struct Entity_properties
{
  uint32_t tags;
};


/*!
  Generic entity storeage. This is the scene graph.
*/
struct Entity_pool
{
  util::generic_id                 *entity_id          = nullptr;
  Entity_properties               *entity_properties  = nullptr;
  char                            *name               = nullptr;
  
  math::transform                 *transform          = nullptr;
  math::aabb                      *aabb               = nullptr;
  
  uint32_t                        *model              = nullptr;
  uint32_t                        *texture            = nullptr;
  
  const uint32_t                  capacity = ENTITY_POOL_SIZE;
  uint32_t                        size = 0;
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

*/
bool
entity_pool_find_index(Entity_pool *pool, const util::generic_id id, size_t *out_index);


/*!
 Push a new entity into the pool.
 \param pool entity_pool.
 \param id The entity id that you wish to insert.
 \return true if it successfully inserted an object.
*/
bool
entity_pool_push_new_entity(Entity_pool *pool, const util::generic_id id);


/*!
  Remove an entity fromt the pool.
  \param pool the entity_pool.
  \param id The entity_id you wish to remove.
  \return true if the id was found and removed.
*/
bool
entity_pool_remove_entity(Entity_pool *pool, const util::generic_id id);


/*!
  Get the name of an entity.
  \param pool The data pool.
  \param id The id of the entity you wish.
  \return returns the name of the entity or nullptr if it couldn't find one.
*/
const char *
entity_pool_get_entity_name(const Entity_pool *pool, const util::generic_id id);


/*
  Update the name of entity. This is primarly about debugging.
*/
void
entity_pool_set_entity_name(const Entity_pool *pool, const util::generic_id id, const char *set_name);


/*!
  Get the entity transform
*/
math::transform
entity_pool_get_transform(Entity_pool *pool,
                          const util::generic_id id);

} // ns


#endif // inc guard