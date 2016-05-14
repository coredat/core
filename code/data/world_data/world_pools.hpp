#ifndef WORLD_POOLS_INCLUDED_2B096887_2423_4C5D_A56C_2F59C3FBFBBC
#define WORLD_POOLS_INCLUDED_2B096887_2423_4C5D_A56C_2F59C3FBFBBC


#include "world_data_fwd.hpp"
#include <utilities/generic_id.hpp>
#include <core/entity/entity_fwd.hpp>
#include <stdint.h>


namespace World_data {


struct World
{
  Transform_data                    *transform            = nullptr;
  Entity_data                       *entity               = nullptr;
  Pending_scene_graph_change_data   *entity_graph_changes = nullptr;
  
  Rigidbody_pool              *rigidbody_pool       = nullptr;
  Camera_pool                 *camera_pool          = nullptr;
  Physics_data                *physics_data         = nullptr;
  Mesh_renderer_data          *mesh_data            = nullptr;
  Text_renderer_data          *text_data            = nullptr;
  
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
                        util::generic_id id);


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
                             util::generic_id out_ids[],
                             const uint32_t size_of_out);
  
  
/*!
  Updates the world data based on scene graph changes.
  \param world_data the world to update.
  \param The graph changes.
*/
void
world_update_scene_graph_changes(World *world_data,
                                 const Pending_scene_graph_change_data *graph_changes);


} // ns



#endif // inc guard