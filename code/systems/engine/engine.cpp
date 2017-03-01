#include "engine.hpp"
#include "physics/physics_tick.hpp"
#include "renderer/renderer_tick.hpp"
#include "renderer/renderer_debug_tick.hpp"
#include "scene_graph/scene_graph_tick.hpp"
#include <data/world_data.hpp>
#include <data/physics/physics.hpp>
#include <utilities/logging.hpp>


namespace {

  std::atomic<bool> init(false);
  
} // anon ns


namespace Engine {


void
initialize(std::shared_ptr<Data::World> world)
{
  init = true;

  Scene_graph_tick::initialize();
  Physics_tick::initialize(world);
  Renderer_tick::initialize();
  Renderer_debug_tick::initialize();
}


void
think(const std::shared_ptr<Data::World> world_data,
      const std::shared_ptr<Data::Context> resource_data,
      const float dt,
      const float running_time,
      const uint32_t width,
      const uint32_t height,
      Tick_information *out_tick_info)
{
  // Gives reader better assumption on what data is being transformed.
  LOG_TODO_ONCE("Engine ticks should pass in only the data they need.");

  /*
    Scene graph must come first. So it can remove any data that shouldn't be processed.
  */
  Scene_graph_tick::think(world_data, out_tick_info);
  
  /*
    We should get to a point where rendering and physics can operate on different threads.
  */
  Physics_tick::think(world_data, dt, out_tick_info);
  
  Data::Physics::world_think(world_data->physics);
  
  Renderer_tick::think(world_data, resource_data, dt, running_time, width, height, out_tick_info);
  Renderer_debug_tick::think(world_data, resource_data, dt, running_time, width, height);
}


void
set_collision_callback(Core::Collision_callback callback)
{
  Physics_tick::collision_callback(callback);
}


void
de_initialize()
{
  init = false;
  
  /*
    Keep this in reverse order to initialize();
  */
  Renderer_tick::de_initialize();
  Physics_tick::de_initialize();
  Scene_graph_tick::de_initialize();
}


} // ns
