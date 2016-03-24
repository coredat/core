#include "world.hpp"
#include <systems/audio/audio.hpp>

#include <core/memory/memory.hpp>
#include <data/data.hpp>

#include <graphics_api/initialize.hpp>
#include <graphics_api/clear.hpp>

#include <sdl_wrapper/sdl_lazy_include.hpp>

#include <renderer/renderer.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
#include <renderer/gui_renderer/gui_renderer.hpp>

namespace Core {


struct World::Impl
{
};


World::World(const World_setup &setup)
: m_impl(new World::Impl)
{
  const size_t chunk_128_mb = 134217728;
  Core::Memory::initialize(chunk_128_mb);

//  sdl::input input;
//  input.set_mouse_hold(true);
  
  Audio::initialize();
  
  Graphics_api::initialize();
  Simple_renderer::initialize();
  Debug_line_renderer::initialize();
  Gui_renderer::initialize();
  
  static World_data::Model_pool model_pool;
  World_data::model_pool_init(&model_pool);
  
  static World_data::Logic_pool logic_pool;
  World_data::logic_pool_init(&logic_pool);
  
  static World_data::Texture_pool texture_pool;
  World_data::texture_pool_init(&texture_pool);
  
  static World_data::Entity_pool world_entities;
  World_data::entity_pool_init(&world_entities);
  
  static World_data::Entity_graph_changes_pool graph_changes;
  World_data::entity_graph_change_pool_init(&graph_changes);
  
  static World_data::Camera_pool camera_pool;
  World_data::camera_pool_init(&camera_pool);
  
  static World_data::Audio_pool audio_pool;
  World_data::audio_pool_init(&audio_pool);
  
  static World_data::Gui_view_pool gui_view_pool;
  World_data::gui_view_pool_init(&gui_view_pool);
  

    m_world_data.audio_pool             = &audio_pool;
    m_world_data.entity_pool            = &world_entities;
    m_world_data.entity_graph_changes   = &graph_changes;
    m_world_data.logic_pool             = &logic_pool;
    m_world_data.texture_pool           = &texture_pool;
    m_world_data.camera_pool            = &camera_pool;
    m_world_data.model_pool             = &model_pool;
    m_world_data.gui_pool               = &gui_view_pool;
  
  World_data::set_world_data(&m_world_data);
  
  // Load some audio data
  {
    Audio::Node_sample_2d node;
    node.chunk_to_play = audio_pool.samples[0].chunk;
  }
  
  // Core Data
  static Core_data::Input_pool core_input;
  
  m_core_data.input_pool = &core_input;

  Core_data::core_data_init(&m_core_data);
  Core_data::set_core_data(&m_core_data);
}


World::~World()
{
}


Entity
World::create_entity()
{
  Entity out_entity;
  World_data::world_create_new_entity(&m_world_data, &out_entity, 99);
  
  return out_entity;
}


Entity_ref
World::find_entity(const char *name)
{
  return Entity_ref();
}


} // ns