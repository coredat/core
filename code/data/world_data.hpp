#ifndef WORLD_DATA_INCLUDED_4FAE9F04_0E15_4C3B_816B_F5C1FEA9EADA
#define WORLD_DATA_INCLUDED_4FAE9F04_0E15_4C3B_816B_F5C1FEA9EADA


#include "data_fwd.hpp"
#include <core/interface/interface_fwd.hpp>
#include <systems/physics/physics_fwd.hpp>
#include <core/entity_id.hpp>
#include <stdint.h>


namespace Data {


struct World
{
  Entity_pool                 *entity_pool = nullptr;
  Entity_graph_changes_pool   *entity_graph_changes = nullptr;
  Logic_pool                  *logic_pool = nullptr;
  Rigidbody_pool              *rigidbody_pool = nullptr;
  Texture_pool                *texture_pool = nullptr;
  Model_pool                  *model_pool = nullptr;
  Camera_pool                 *camera_pool = nullptr;
  Physics::World              *physics_world = nullptr;
}; // struct



/*!
 Create a new entity. This abstraction will insert the correct
 data elements into to the world.
 \param world_data Is the world in which we are entering data.
 \param out_entity is the result of the find.
 \param type_id The id of the element we are inserting into the world.
 \return returns true if we created the entity.
*/
bool
world_create_new_entity(World *world_data, Core::Entity *out_entity, const uint32_t type_id);


/*!
  Find an existing entity.
  \param world_data is the world in which to search.
  \param out_entity is the result of the find.
  \param id is the entity we are searching.
  \return true or false depending on if we found it or not.
*/
bool
world_find_entity(World *world_data, Core::Entity *out_entity, const Core::Entity_id id);



} // ns


#endif // inc guard