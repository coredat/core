#ifndef WORLD_POOLS_INCLUDED_2B096887_2423_4C5D_A56C_2F59C3FBFBBC
#define WORLD_POOLS_INCLUDED_2B096887_2423_4C5D_A56C_2F59C3FBFBBC


#include "world_data_fwd.hpp"
#include <core/entity_id.hpp>
#include <core/entity/entity_fwd.hpp>
#include <stdint.h>


namespace World_data {


struct World
{
//  Audio_pool                  *audio_pool           = nullptr;
  Entity_pool                 *entity_pool          = nullptr;
  Entity_graph_changes_pool   *entity_graph_changes = nullptr;
  Rigidbody_pool              *rigidbody_pool       = nullptr;
 // Texture_pool                *texture_pool         = nullptr;
//  Model_pool                  *model_pool           = nullptr;
  Camera_pool                 *camera_pool          = nullptr;
  Gui_view_pool               *gui_pool             = nullptr;
  Application_window          *app_window           = nullptr;
  Physics_data                *physics_data         = nullptr;
  
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
world_create_new_entity(World *world_data,
                        Core::Entity *out_entity,
                        const uint32_t type_id);


/*!
  Find an existing entity.
  \param world_data is the world in which to search.
  \param out_entity is the result of the find.
  \param id is the entity we are searching.
  \return true or false depending on if we found it or not.
*/
bool
world_find_entity(World *world_data,
                  Core::Entity_ref *out_entity,
                  const Core::Entity_id id);


/*!
  Search for an entity that has a corrisponding tag.
  \param world_data is the world to search.
  \param tag the tag to find.
  \param out_entities_for_tag number of entities we found.
  \param out_ids[] where to place results.
  \param size_of_out size of the result array.
*/
void
world_find_entities_with_tag(World *world_data,
                             const uint32_t tag,
                             uint32_t *out_entities_for_tag,
                             Core::Entity_id out_ids[],
                             const uint32_t size_of_out);
  
  
/*!
  Updates the world data based on scene graph changes.
  \param world_data the world to update.
  \param The graph changes.
*/
void
world_update_scene_graph_changes(World *world_data,
                                 const Entity_graph_changes_pool *graph_changes);


} // ns



#endif // inc guard