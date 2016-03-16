#ifndef LOGIC_POOL_INCLUDED_2879D607_1430_4D5C_8806_974D5294E9F5
#define LOGIC_POOL_INCLUDED_2879D607_1430_4D5C_8806_974D5294E9F5


#include <core/interface/component.hpp>
#include <core/entity_id.hpp>
#include <stddef.h>


#define LOGIC_POOL_NUMBER_OF_SCRIPTS 512
#define LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE 128


namespace World_data {


/*
  Logic pool stores the application layers code.
  This memory pool allows us to store ptrs to the registered hooks.
  This allows us to only call hooks that have been registered saving on time.
  
  This is prob the most dangerous thing right now.
*/
namespace Logic_hook {

  enum ENUM
  {
    on_start          = 1 << 0,
    on_early_update   = 1 << 1,
    on_update         = 1 << 2,
    on_end            = 1 << 3,
    
    to_destroy        = 1 << 4, // This is a special flag that we use to identity what objects need to be removed.
  };

} // ns


struct Logic_pool
{
  Core::Entity_id               *entity_id;
  uint32_t                      *regd_hook;
  uint8_t                       *object_store;
  uint32_t                      size = 0;
  const uint32_t                capacity = LOGIC_POOL_NUMBER_OF_SCRIPTS;
};


/*!
  Initialises the pool.
  \param pool The pool to initialize.
  \param allocate_memory Set false if you manually allocate the memory for the pool.
*/
void
logic_pool_init(Logic_pool *pool,
                const bool allocate_memory = true);


/*!
  Returns a pointer to a valid location for a new object
*/
void*
logic_pool_get_slot(Logic_pool *pool, const Core::Entity_id id);


/*!
  Find a component.
*/
void*
logic_pool_get_component(Logic_pool *pool, const Core::Entity_id id);

/*!
  Return how many components are attached to an entity.
*/
uint32_t
logic_pool_get_slot_count(Logic_pool *pool, const Core::Entity_id id);


/*!
  Removes all the slots associated with an entity.
*/
void
logic_pool_clean_up(Logic_pool *pool);



// ** EVENT HOOKS ** //
/* These methods call logic event hooks. */


/*!
  Calls the on_start hooks of any pending logic scripts.
  After calling this the objects are removed from this data source and cannot be re-added.
*/
void
logic_pool_on_start_hook(Logic_pool *pool);


/*!
  Calls the on_update hooks of any pending logic scripts.
*/
void
logic_pool_on_early_update_hook(Logic_pool *pool, const float delta_time);


/*!
  Calls the on_update hooks of any pending logic scripts.
*/
void
logic_pool_on_update_hook(Logic_pool *pool, const float delta_time);


/*!
  Call the on_collision hooks.
*/
void
logic_pool_on_collision_hook(Logic_pool *pool, const Core::Entity_id id_a, const Core::Entity_id id_b);

/*!
  Call the on end hook for objects that are to be removed.
*/
void
logic_pool_on_end_hook(Logic_pool *pool, const Core::Entity_id ids[], const uint32_t number_of_entities);

} // ns


#endif // inc guard