 #include "world.hpp"
#include <core/world/world_setup.hpp>

#include <core/memory/memory.hpp>

#include <graphics_api/initialize.hpp>
#include <graphics_api/clear.hpp>

#include <core/entity/entity.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/world/detail/world_detail.hpp>

#include <renderer/renderer.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>
#include <renderer/gui_renderer/gui_renderer.hpp>

#include <data/world_data/physics_data.hpp>

#include <utilities/logging.hpp>
#include <core/entity_id.hpp>
#include <math/math.hpp> // remove
#include <vector> // remove

#include <systems/physics_engine/broadphase/sweep.hpp>
#include <systems/physics_engine/broadphase/prune.hpp>
#include <systems/physics_engine/collision/aabb_overlap.hpp>
#include <systems/physics_engine/collision/collision_pairs.hpp>
#include <systems/physics_engine/physics_engine.hpp>


namespace Core {


struct World::Impl
{
  std::shared_ptr<World_detail::World_data> world_data = std::make_shared<World_detail::World_data>();
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
  
  static World_data::Physics_data physics_data;
  World_data::physics_init(&physics_data, 2048);
  
  
  m_impl->world_data->data.entity_pool          = &world_entities;
  m_impl->world_data->data.entity_graph_changes = &graph_changes;
  m_impl->world_data->data.camera_pool          = &camera_pool;
  m_impl->world_data->data.gui_pool             = &gui_view_pool;
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
  // Push in new phy entities.
  World_data::world_update_scene_graph_changes(&m_impl->world_data->data, m_impl->world_data->data.entity_graph_changes);
  
  // Reset the entity pool for new changes.
  World_data::entity_graph_change_pool_init(m_impl->world_data->data.entity_graph_changes);
}


void
//World::get_overlapping_aabbs(const std::function<void(const Entity_ref ref_a, const Entity_ref ref_b)> &callback)
World::get_overlapping_aabbs(const std::function<void(const Physics_engine::Collision_pair pairs[],
                                                      const uint32_t number_of_pairs)> &callback)
{
  // Check we have a callback.
  if(!callback) { return; }

  // TODO need this on a memory pool, get it out of the stack.
  const World_data::Entity_pool *entity_data = m_impl->world_data->data.entity_pool;
  
  // Create aabbs with tranforms
//  math::aabb transformed_aabbs[2048];
  
  
  Physics::Broadphase::Sweep sweep;
  Physics::Broadphase::sweep_init(&sweep, entity_data->size);
  Physics::Broadphase::sweep_calculate(&sweep, entity_data->aabb, entity_data->size);
  
  Physics::Broadphase::Prune prune;
  Physics::Broadphase::prune_init(&prune, &sweep);
  Physics::Broadphase::prune_calculate(&prune, &sweep);

  
  // Prune out
  std::vector<Core::Entity_id> id;
  std::vector<math::aabb> boxes;
  
  uint32_t prune_stack = 0;
  
  for(uint32_t i = 0; i < entity_data->size; ++i)
  {
    if(prune_stack < prune.size && i == prune.ids[prune_stack])
    {
      ++prune_stack;
      continue;
    }
    
    id.push_back(entity_data->entity_id[i]);
    boxes.push_back(entity_data->aabb[i]);
  }
  
  // Calculate collisions
  Physics::Collision::Pairs out_pairs;
  Physics::Collision::pairs_init(&out_pairs, 2048);
  
  Physics::Collision::aabb_calculate_overlaps_pairs(&out_pairs, boxes.data(), boxes.size());
  
  
  Physics::Broadphase::sweep_free(&sweep);
  Physics::Broadphase::prune_free(&prune);
  Physics::Collision::pairs_free(&out_pairs);

  
//  static World_data::Sweep_and_prune sweep; // Temp we need to hold onto the memory!
//  World_data::sweep_and_prune_create(&sweep,
//                                     entity_data->entity_id,
//                                     transformed_aabbs.data(),
//                                     entity_data->size,
//                                     math::aabb_init(math::vec3_init(size, size, size),
//                                                     math::vec3_init(-size, -size, -size)));
//  
//  static std::vector<Physics_engine::Collision_pair> collision_pairs;
//  collision_pairs.reserve(2048);
//  uint32_t start_point = 0;

  
//  for(uint32_t i = 0; i < 64 * 64 * 64; ++i)
//  {
//    if(sweep.bucket[i].entity_bounds.size() > 2)
//    {
//      const std::string size = "bucket " + std::to_string(i) + " size:" + std::to_string(sweep.bucket[i].entity_bounds.size());
//      
//      //LOG_INFO(size.c_str())
//    }
//  }
//  }

//  callback(out_collisions, out_number_of_collisions);

  // Log a warning if we have max out the collision buffer.
//  {
//    static int log_max_warning_once = 0;
//    if(out_number_of_collisions >= max_collisions && !log_max_warning_once)
//    {
//      log_max_warning_once = 1;
//      LOG_WARNING("Collisions have maxed out.")
//    }
//  }
}


Entity_ref
World::find_entity_by_name(const char *name)
{
  return Entity_ref();
}


Entity_ref
World::find_entity_by_id(const Core::Entity_id id)
{
  return Entity_ref(id, &m_impl->world_data->data);
}


std::shared_ptr<const World_detail::World_data>
World::get_world_data() const
{
  assert(m_impl);
  return m_impl->world_data;
}



} // ns