#include "engine.hpp"
#include "physics/physics_tick.hpp"
#include "renderer/renderer_tick.hpp"
#include "scene_graph/scene_graph_tick.hpp"


namespace {

  std::atomic<bool> init(false);
  
} // anon ns


namespace Engine {


void
initialize()
{
  init = true;

  Scene_graph_tick::initialize();
  Physics_tick::initialize();
  Renderer_tick::initialize();
}


void
think(const std::shared_ptr<World_data::World> world_data, const float dt)
{
  // Scene graph must be first so that the rest is working on correct data.
  Scene_graph_tick::think();
  
  Physics_tick::think(world_data, dt);
  
  Renderer_tick::think();
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
}


} // ns