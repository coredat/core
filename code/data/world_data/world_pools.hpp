#ifndef WORLD_POOLS_INCLUDED_2B096887_2423_4C5D_A56C_2F59C3FBFBBC
#define WORLD_POOLS_INCLUDED_2B096887_2423_4C5D_A56C_2F59C3FBFBBC


#include "world_data_fwd.hpp"
#include <utilities/generic_id.hpp>
#include <core/entity/entity_fwd.hpp>
#include <3rdparty/qu3e/q3.h>
#include <stdint.h>


namespace World_data {


struct World
{
  Data::Transform_data                    *transform            = nullptr;
  Entity_data                       *entity               = nullptr;
  Pending_scene_graph_change_data   *entity_graph_changes = nullptr;
  Camera_data                       *camera_data          = nullptr;
  Physics_data                      *physics_data         = nullptr;
  Renderer_mesh_data                *mesh_data            = nullptr;
  Renderer_text_draw_calls_data     *text_data            = nullptr;
  
  // RB
//  q3Scene scene = q3Scene(1.f/60.f); // Should this be here?
  q3Scene *scene;// = q3Scene(1.f/60.f); // Should this be here?
  
  explicit World(const uint32_t size_hint);
  ~World();
  
}; // struct



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