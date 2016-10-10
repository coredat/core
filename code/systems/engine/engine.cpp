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
  Scene_graph_tick::think();
  Physics_tick::think();
  Renderer_tick::think();
}


void
de_initialize()
{
  init = false;
}


} // ns