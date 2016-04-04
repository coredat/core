#include "world.hpp"
#include <core/world/world_setup.hpp>
#include <systems/audio/audio.hpp>

#include <core/memory/memory.hpp>
#include <data/data.hpp>

#include <graphics_api/initialize.hpp>
#include <graphics_api/clear.hpp>

#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>


#include <data/world_data/world_data.hpp>


#include <sdl_wrapper/sdl_lazy_include.hpp>

#include <renderer/renderer.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
#include <renderer/gui_renderer/gui_renderer.hpp>

#include <utilities/logging.hpp>

namespace Core {


struct World::Impl
{
  Core_data::Core    core_data;
  World_data::World  world_data;
};


World::World(const World_setup &setup)
: m_impl(new World::Impl)
{
  const size_t chunk_128_mb = 134217728;
  Core::Memory::initialize(chunk_128_mb);

  LOG_TODO("Remove static data stores")
  static World_data::Entity_pool world_entities;
  World_data::entity_pool_init(&world_entities);
  
  static World_data::Entity_graph_changes_pool graph_changes;
  World_data::entity_graph_change_pool_init(&graph_changes);
  
  static World_data::Camera_pool camera_pool;
  World_data::camera_pool_init(&camera_pool);
  
  static World_data::Gui_view_pool gui_view_pool;
  World_data::gui_view_pool_init(&gui_view_pool);
  
  static World_data::Application_window app_window;
  World_data::application_window_init(&app_window);
  
  m_impl->world_data.entity_pool            = &world_entities;
  m_impl->world_data.entity_graph_changes   = &graph_changes;
  m_impl->world_data.camera_pool            = &camera_pool;
  m_impl->world_data.gui_pool               = &gui_view_pool;
  
  World_data::set_world_data(&m_impl->world_data);
}


World::~World()
{
}


void
World::think(const float dt)
{
}


Entity
World::create_entity()
{
  Entity out_entity;
  World_data::world_create_new_entity(&m_impl->world_data, &out_entity, 99);
  
  return out_entity;
}


Entity_ref
World::find_entity(const char *name)
{
  return Entity_ref();
}


} // ns