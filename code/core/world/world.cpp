#include <core/world/world.hpp>
#include <core/context/context.hpp>
#include <core/physics/collision_pair.hpp>
#include <core/transform/transform.hpp>

#include <debug_gui/debug_menu.hpp>

#include <renderer/simple_renderer/simple_renderer_node.hpp>
#include <renderer/simple_renderer/simple_renderer.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer_node.hpp>
#include <renderer/debug_line_renderer/debug_line_renderer.hpp>

#include <data/world_data/physics_data.hpp>
#include <data/world_data/transform_data.hpp>
#include <data/world_data/entity_data.hpp>
#include <data/world_data/world_data.hpp>
#include <data/global_data/resource_data.hpp>
#include <data/global_data/memory_data.hpp>

#include <systems/physics_engine/broadphase/sweep.hpp>
#include <systems/physics_engine/broadphase/prune.hpp>
#include <systems/physics_engine/collision/aabb_overlap.hpp>
#include <systems/physics_engine/collision/collision_pairs.hpp>
#include <systems/physics_engine/collision/axis_collidable.hpp>

#include <systems/renderer_material/material.hpp>
#include <systems/renderer_material/material_renderer.hpp>
#include <systems/renderer_post/post_process.hpp>
#include <systems/renderer_material/material_renderer.hpp>

#include <transformations/physics/overlapping_aabb.hpp>
#include <transformations/rendering/material_renderer.hpp>
#include <transformations/camera/cam_priorities.hpp>
#include <transformations/rendering/render_scene.hpp>

#include <utilities/timer.hpp>
#include <utilities/logging.hpp>
#include <utilities/generic_id.hpp>
#include <utilities/conversion.hpp>

#include <graphics_api/initialize.hpp>
#include <graphics_api/clear.hpp>


namespace Core {


struct World::Impl
{
  std::shared_ptr<World_data::World> world_data;
  util::timer dt_timer;
  float       dt           = 0.f;
  float       dt_mul       = 1.f;
  float       running_time = 0.f;
};


World::World(const Context &ctx, const World_setup setup)
: m_impl(new World::Impl)
{
  m_impl->world_data = std::make_shared<World_data::World>(setup.entity_pool_size);
  
  Simple_renderer::initialize();
  Debug_line_renderer::initialize();
  
  ::Material_renderer::initialize();
  ::Post_renderer::initialize();
  
  m_impl->dt_timer.start();
}


World::~World()
{
}


float
World::get_delta_time() const
{
  return m_impl->dt * m_impl->dt_mul;
}


void
World::set_delta_time_multiplier(const float multiplier)
{
  assert(m_impl);
  
  m_impl->dt_mul = multiplier;
}
  
  
float
World::get_delta_time_multiplier() const
{
  assert(m_impl);
  
  return m_impl->dt_mul;
}


uint32_t
World::get_time_running() const
{
  assert(m_impl);
  
  return static_cast<uint32_t>(m_impl->running_time * 1000);
}


void
World::think()
{
  // Calculate delta_time
  {
    const util::milliseconds frame_time = m_impl->dt_timer.split();
    m_impl->dt = static_cast<float>(frame_time) / 1000.f;
    
    m_impl->running_time += m_impl->dt;
  }
  
  // Update world
  auto data = m_impl->world_data;
  auto graph_changes = m_impl->world_data->entity_graph_changes;

  // Push in new phy entities.
  World_data::world_update_scene_graph_changes(data.get(), graph_changes);
  
  // Reset the entity pool for new changes.
  World_data::pending_scene_graph_change_reset(graph_changes);
  
  
  auto resources = Resource_data::get_resources();
  auto world = m_impl->world_data.get();

  
  /*
    Camera Runs
    --
    For each camera we need a to create a camera run.
    Will will render all the things it is interested in.
    
    TODO
    --
    Can we async this?
  */
  auto cam_data = world->camera_data;

  World_data::data_lock(cam_data);
  uint32_t number_of_cam_runs = 0;
  
  Camera_utils::Cam_run *cam_runs = SCRATCH_ALIGNED_ALLOC(Camera_utils::Cam_run, cam_data->size);
  {
    number_of_cam_runs = cam_data->size;
    
    // Generate cam_run data
    {
      Core::Transform *cam_transforms = SCRATCH_ALIGNED_ALLOC(Core::Transform, cam_data->size);
      Camera_utils::get_camera_transforms(world->transform, cam_data->property_entity_id, cam_transforms, cam_data->size);
      Camera_utils::calculate_camera_runs(cam_data,
                                          Resource_data::get_resources()->texture_data,
                                          cam_transforms,
                                          cam_runs,
                                          cam_data->size);
    }
  }
  
  World_data::data_unlock(cam_data);
  
  
  /*
    Generate the Draw call list
    --
    This list is every draw call that needs to be rendered.
    
    TODO
    --
    Can we async this?
  */
  ::Material_renderer::Draw_call *draw_calls = SCRATCH_ALIGNED_ALLOC(::Material_renderer::Draw_call, world->mesh_data->size);
  {
    for(uint32_t i = 0; i < world->mesh_data->size; ++i)
    {
      // Draw call from the data.
      const World_data::Mesh_renderer_draw_call *draw_call_data = &world->mesh_data->property_draw_call[i];

      // No model? keep moving.
      if(!draw_call_data->model_id)
      {
        continue;
      }
      
      // Get the hardware mesh.
      // Possible extension. We could also process these based on how far away the camera is.
      Resource_data::mesh_data_get_property_mesh(resources->mesh_data, draw_call_data->model_id, &draw_calls[i].mesh);
      
      const float *world_mat = draw_call_data->world_matrix;

      memcpy(&draw_calls[i], world_mat, sizeof(float) * 16);
      
      // Get cull mask.
      // This isn't particularly nice. We should already have this data to save us looking for it.
      const util::generic_id entity_id = world->mesh_data->renderer_mesh_id[i];
      World_data::entity_data_get_property_tag(world->entity, entity_id, &draw_calls[i].cull_mask);
    }
  }
  
  
  /*
    Render the world
    --
    Takes the camera, and draw calls and renders the world accordingly.
  */
  uint32_t number_of_draw_calls = 0;
  Rendering::render_main_scene(m_impl->dt,
                               m_impl->running_time,
                               world->mesh_data,
                               resources->material_data,
                               resources->post_data,
                               cam_runs,
                               number_of_cam_runs,
                               draw_calls,
                               world->mesh_data->size,
                               &number_of_draw_calls);

  /*
    Debug Menu
    --
    Shows the debugging menu bar at the top of the screen.
  */
  #ifdef CORE_DEBUG_MENU
  {
    Debug_menu::display_global_data_menu();
    Debug_menu::display_world_data_menu(m_impl->world_data.get(),
                                        m_impl->dt,
                                        m_impl->dt_mul,
                                        number_of_draw_calls,
                                        number_of_cam_runs);
  }
  #endif
}


void
World::get_overlapping_aabbs(const std::function<void(const Core::Collision_pair pairs[],
                                                      const uint32_t number_of_pairs)> &callback)
{
  // Check we have a callback.
  if(!callback) { return; }
  
  LOG_TODO_ONCE("This can move out into a transform, or done on a thread during think.");

  const World_data::Physics_data *data = m_impl->world_data->physics_data;
  
  math::aabb *bounds = reinterpret_cast<math::aabb*>(::Memory::scratch_alloc_aligned(sizeof(math::aabb) * data->size));
  
  Transformation::calculate_positional_aabb(data->property_aabb_collider,
                                            data->property_transform,
                                            bounds,
                                            data->size);
  
  Physics::Broadphase::Sweep sweep;
  Physics::Broadphase::sweep_init(&sweep, data->size);
  
  Physics::Broadphase::sweep_calculate(&sweep, data->property_transformed_aabb_collider, data->size);
  
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
    
    id.push_back(data->physics_id[i]);
    
    math::aabb box_copy(data->property_aabb_collider[i]);
    uint64_t collision_mask(data->property_collision_id[i]);
    math::aabb_scale(box_copy, data->property_transform[i].scale);
    math::aabb_set_origin(box_copy, data->property_transform[i].position);
    
    boxes.push_back(Physics::Collision::Axis_collidable{{collision_mask}, box_copy});
  }
  
  assert(prune_stack == prune.size);
  
  // Calculate collisions
  Physics::Collision::Pairs out_pairs;
  Physics::Collision::pairs_init(&out_pairs, 2048 * 10);
  
  Physics::Collision::aabb_calculate_overlaps_pairs(&out_pairs, boxes.data(), boxes.size());
  
  static Core::Collision_pair *pairs = nullptr;
  if(!pairs)
  {
    pairs = new Core::Collision_pair[2048 * 10];
  }
  
  uint32_t number_of_pairs = 0;
  const uint32_t max_pairs = 2048 * 10;
  
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
  
  
  Physics::Broadphase::sweep_free(&sweep);
  Physics::Broadphase::prune_free(&prune);
  Physics::Collision::pairs_free(&out_pairs);
}


Entity_ref
World::find_entity_by_id(const util::generic_id id) const
{
  return Entity_ref(id, *const_cast<World*>(this));
}


void
World::find_entities_by_tag(const uint32_t tag_id,
                            Entity_ref **out_array,
                            size_t *out_array_size)
{
  static Entity_ref found_ents[1024];
  static size_t count = 0;
  
  count = 0;
  
  // Loop through entity data and find entities.
  auto data = m_impl->world_data->entity;
  
  data_lock(data);
  
  for(uint32_t i = 0; i < data->size; ++i)
  {
    if(data->property_tag[i] & tag_id)
    {
      found_ents[count] = Entity_ref(data->entity_id[i], *const_cast<World*>(this));
      count++;
    }
  }
  
  data_unlock(data);
  
  *out_array = found_ents;
  *out_array_size = count;
}


std::shared_ptr<const World_data::World>
World::get_world_data() const
{
  assert(m_impl);
  return m_impl->world_data;
}


std::shared_ptr<World_data::World>
World::get_world_data()
{
  assert(m_impl);
  return m_impl->world_data;
}


} // ns