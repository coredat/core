#ifndef WORLD_POOLS_INCLUDED_2B096887_2423_4C5D_A56C_2F59C3FBFBBC
#define WORLD_POOLS_INCLUDED_2B096887_2423_4C5D_A56C_2F59C3FBFBBC


#include <core/entity/entity_fwd.hpp>
#include <data/fwd_dec_data.hpp>
#include <data/graph/fwd.hpp>
#include <data/physics/fwd.hpp>
#include <data/renderers/text/fwd.hpp>


namespace Data {


struct World
{
  Pending_entity_removal_data   *entity_removal  = nullptr;
  Camera_data                   *camera_data     = nullptr;
  Rigidbody_data                *rigidbody_data  = nullptr;
  Trigger_data                  *trigger_data    = nullptr;
  Mesh_draw_call_data           *mesh_data       = nullptr;
  Text_draw_call_data           *text_data       = nullptr;
  Collision_data                *collision_data  = nullptr;
  Light_data                    *light_data      = nullptr;
  
  Text_renderer_data            *text_renderer   = nullptr;
  Graph::Graph_data             *scene_graph     = nullptr;
  Physics::Physics_data         *physics         = nullptr;
  
  uint32_t world_instance_id = 0;
  
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  
  explicit World(const uint32_t world_instance_id, const uint32_t size_hint);
  ~World();
  
}; // struct



// TODO: This needs to be a transform.
/*!
  Updates the world data based on scene graph changes.
  \param world_data the world to update.
  \param The graph changes.
*/
void
world_update_scene_graph_changes(World *world_data,
                                 const Data::Pending_entity_removal_data *graph_changes);


} // ns



#endif // inc guard
