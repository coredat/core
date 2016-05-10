 #include "world.hpp"
#include <core/world/world_setup.hpp>

#include <core/memory/memory.hpp>

#include <graphics_api/initialize.hpp>
#include <graphics_api/clear.hpp>

#include <core/physics/collision_pair.hpp>
#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/world/detail/world_detail.hpp>

#include <renderer/renderer.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
#include <renderer/gui_renderer/gui_renderer.hpp>

#include <data/world_data/physics_data.hpp>

#include <utilities/logging.hpp>
#include <utilities/generic_id.hpp>
#include <math/math.hpp> // remove
#include <vector> // remove

#include <systems/physics_engine/broadphase/sweep.hpp>
#include <systems/physics_engine/broadphase/prune.hpp>
#include <systems/physics_engine/collision/aabb_overlap.hpp>
#include <systems/physics_engine/collision/collision_pairs.hpp>
#include <systems/physics_engine/physics_engine.hpp>
#include <systems/physics_engine/collision/axis_collidable.hpp>

#include <3rdparty/imgui/imgui.h>
#include <3rdparty/imgui/imgui_impl_sdl_gl3.h>


namespace Core {


struct World::Impl
{
  std::shared_ptr<World_detail::Data> world_data = std::make_shared<World_detail::Data>();
};


World::World(const World_setup setup)
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
    
  static World_data::Physics_data physics_data;
  World_data::physics_init(&physics_data, 2048);
  
  m_impl->world_data->data.entity_pool          = &world_entities;
  m_impl->world_data->data.entity_graph_changes = &graph_changes;
  m_impl->world_data->data.camera_pool          = &camera_pool;
  m_impl->world_data->data.physics_data         = &physics_data;
  
  LOG_TODO("We can store the data directly and get rid of ::World_data::World")
  World_data::set_world_data(&m_impl->world_data->data);
}


World::~World()
{
}


void
World::think(const float dt)
{
  auto data = &m_impl->world_data->data;
  auto graph_changes = m_impl->world_data->data.entity_graph_changes;

  // Push in new phy entities.
  World_data::world_update_scene_graph_changes(data, graph_changes);
  
  // Reset the entity pool for new changes.
  World_data::entity_graph_change_pool_init(graph_changes);
  
  
  ImGui::Begin("Entities");
  
  // Loop through all
  for(uint32_t i = 0; i < data->entity_pool->size; ++i)
  {
    const char *name = World_data::entity_pool_get_entity_name(data->entity_pool, data->entity_pool->entity_id[i]);
    
    std::string number = std::to_string(data->entity_pool->entity_id[i]);
  
    ImGui::LabelText(name, number.c_str());
  }
  
  ImGui::End();
}


void
World::get_overlapping_aabbs(const std::function<void(const Core::Collision_pair pairs[],
                                                      const uint32_t number_of_pairs)> &callback)
{
  // Check we have a callback.
  if(!callback) { return; }

  const World_data::Entity_pool *entity_data = m_impl->world_data->data.entity_pool;
  const World_data::Physics_data *data = m_impl->world_data->data.physics_data;
  
  static std::vector<math::aabb> boundings;
  boundings.clear();
  
  for(uint32_t i = 0; i < data->size; ++i)
  {
    math::aabb box_copy(data->aabb_collider[i]);
    math::aabb_scale(box_copy, data->transform[i].scale);
    math::aabb_set_origin(box_copy, data->transform[i].position);
    
    boundings.push_back(box_copy);
  }
  
  Physics::Broadphase::Sweep sweep;
  Physics::Broadphase::sweep_init(&sweep, data->size);
  Physics::Broadphase::sweep_calculate(&sweep, boundings.data(), boundings.size());
  
  assert(sweep.size == boundings.size());
  
  Physics::Broadphase::Prune prune;
  Physics::Broadphase::prune_init(&prune, &sweep);
  Physics::Broadphase::prune_calculate(&prune, &sweep);

  // Prune out
  static std::vector<util::generic_id> id;
  static std::vector<Physics::Collision::Axis_collidable> boxes;
  
  id.clear();
  boxes.clear();
  
  uint32_t prune_stack = 0;
    
  for(uint32_t i = 0; i < data->size; ++i)
  {
    if(prune_stack < prune.size && i == prune.ids[prune_stack])
    {
      ++prune_stack;
            
      continue;
    }
    
    id.push_back(data->entity_id[i]);
    
    math::aabb box_copy(data->aabb_collider[i]);
    uint64_t collision_mask(data->collision_id[i]);
    math::aabb_scale(box_copy, data->transform[i].scale);
    math::aabb_set_origin(box_copy, data->transform[i].position);
    
    boxes.push_back(Physics::Collision::Axis_collidable{collision_mask, box_copy});
  }
  
  assert(prune_stack == prune.size);
  
  // Calculate collisions
  Physics::Collision::Pairs out_pairs;
  Physics::Collision::pairs_init(&out_pairs, 2048 * 10);
  
  Physics::Collision::aabb_calculate_overlaps_pairs(&out_pairs, boxes.data(), boxes.size());
  
  static Core::Collision_pair *pairs = nullptr;
  if(!pairs)
  {
    pairs = new Core::Collision_pair[2048];
  }
  
  uint32_t number_of_pairs = 0;
  const uint32_t max_pairs = 2048;
  
  if(out_pairs.size)
  {
    // Build collision pairs array.
    for(int32_t i = 0; i < std::min(out_pairs.size, max_pairs); ++i)
    {
      const uint32_t index_a = out_pairs.pair_arr[i].a;
      const uint32_t index_b = out_pairs.pair_arr[i].b;

      pairs[number_of_pairs++] = Core::Collision_pair{find_entity_by_id(id[index_a]), find_entity_by_id(id[index_b])};
    }
  
    callback(pairs, number_of_pairs);
  }
  
  ImGui::Begin("Sweep And Prune");
  
  ImVec2 window_size = ImGui::GetWindowSize();
  ImVec2 cursor = ImGui::GetCursorScreenPos();
  
  // Loop through all
  for(uint32_t i = 0; i < data->size; ++i)
  {
    const math::aabb *box_coll = &data->aabb_collider[i];
    // z x
    {
      
    }
  }
  
  ImGui::End();
  
  Physics::Broadphase::sweep_free(&sweep);
  Physics::Broadphase::prune_free(&prune);
  Physics::Collision::pairs_free(&out_pairs);
}


Entity_ref
World::find_entity_by_id(const util::generic_id id) const
{
  return Entity_ref(id, *const_cast<World*>(this));
}


std::shared_ptr<const World_detail::Data>
World::get_world_data() const
{
  assert(m_impl);
  return m_impl->world_data;
}


std::shared_ptr<World_detail::Data>
World::get_world_data()
{
  assert(m_impl);
  return m_impl->world_data;
}


} // ns