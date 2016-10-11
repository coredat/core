#include "scene_graph_tick.hpp"

#include <graphics_api/initialize.hpp>
#include <graphics_api/clear.hpp>
#include <graphics_api/command_buffer.hpp>



namespace Engine {
namespace Scene_graph_tick {


void
initialize()
{
}


void
think()
{
  // Otherwise we might process things that the calling code as already removed.
  {
    // Update world
    auto graph_changes = world->entity_graph_changes;

    // Push in new phy entities.
    World_data::world_update_scene_graph_changes(world.get(), graph_changes);
    
    // Reset the entity pool for new chandges.
    World_data::pending_scene_graph_change_reset(graph_changes);
  }
}


void
de_initialize()
{
}


} // ns
} // ns