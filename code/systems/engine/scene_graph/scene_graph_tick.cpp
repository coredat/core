#include "scene_graph_tick.hpp"

#include <graphics_api/initialize.hpp>
#include <graphics_api/clear.hpp>
#include <graphics_api/command_buffer.hpp>
#include <data/world/pending_entity_removal_data.hpp>



namespace Engine {
namespace Scene_graph_tick {


void
initialize()
{
}


void
think(std::shared_ptr<Data::World> world, Tick_information *tick_info)
{
  // Otherwise we might process things that the calling code as already removed.
  {
    // Update world
    auto graph_changes = world->entity_removal;

    // Push in new phy entities.
    Data::world_update_scene_graph_changes(world.get(), graph_changes);
    
    // Reset the entity pool for new chandges.
    Data::pending_entity_removal_clear(graph_changes);
  }
}


void
de_initialize()
{
}


} // ns
} // ns