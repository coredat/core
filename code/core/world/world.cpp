#include <core/world/world.hpp>
#include <core/context/context.hpp>
#include <core/physics/collision_pair.hpp>
#include <core/physics/collision.hpp>
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
#include <data/world_data/renderer_text_draw_calls_data.hpp>
#include <data/global_data/resource_data.hpp>
#include <data/global_data/memory_data.hpp>

#include <systems/physics_engine/collision/aabb_overlap.hpp>
#include <systems/physics_engine/collision/collision_pairs.hpp>

#include <systems/renderer_material/material.hpp>
#include <systems/renderer_material/material_renderer.hpp>
#include <systems/renderer_post/post_process.hpp>
#include <systems/renderer_material/material_renderer.hpp>
#include <systems/renderer_aabb/renderer_aabb.hpp>
#include <systems/renderer_text/text_renderer.hpp>

#include <transformations/physics/overlapping_aabb.hpp>
#include <transformations/physics/update_world.hpp>
#include <transformations/rendering/material_renderer.hpp>
#include <transformations/camera/cam_priorities.hpp>
#include <transformations/rendering/render_scene.hpp>
#include <transformations/entity/entity_transform.hpp>

#include <utilities/timer.hpp>
#include <utilities/logging.hpp>
#include <utilities/generic_id.hpp>
#include <utilities/conversion.hpp>

#include <graphics_api/initialize.hpp>
#include <graphics_api/clear.hpp>

#include <3rdparty/qu3e/q3.h>
#include <transformations/physics/q3_math_extensions.hpp>


namespace Core {


struct World::Impl
{
  std::shared_ptr<World_data::World> world_data;
  Core::Context *context = nullptr;
  util::timer dt_timer;
  float       dt           = 0.f;
  float       dt_mul       = 1.f;
  float       running_time = 0.f;
  Collision_callback collision_callback = nullptr;
};


World::World(Context &ctx, const World_setup setup)
: m_impl(new World::Impl)
{
  LOG_TODO_ONCE("World should be 'moveable'");
  
  m_impl->world_data = std::make_shared<World_data::World>(setup.entity_pool_size);
  m_impl->context = &ctx;
  
  Simple_renderer::initialize(); // TODO: This can be removed I think, largely superceded by mat renderer
  Debug_line_renderer::initialize();
  Aabb_renderer::initialize();
  
  ::Text_renderer::initialize();
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
  // DONT MOVE.
  // These are handy for debugging, lldb sometimes has hard time with unique_ptr.
  auto resources = Resource_data::get_resources();
  auto world = m_impl->world_data.get();

  // THIS MUST BE FIRST STATE CHANGES!
  // Otherwise we might process things that the calling code as already removed.
  {
    // Update world
    auto data = m_impl->world_data;
    auto graph_changes = m_impl->world_data->entity_graph_changes;

    // Push in new phy entities.
    World_data::world_update_scene_graph_changes(data.get(), graph_changes);
    
    // Reset the entity pool for new chandges.
    World_data::pending_scene_graph_change_reset(graph_changes);
  }
  
  LOG_TODO_ONCE("scratch code to get rb transforms");
  // Raycast test
  {
    class Raycast : public q3QueryCallback
    {
      bool ReportShape(q3Box *shape)
      {
        auto parent = shape->body;
        auto user_data = util::generic_id_from_ptr(parent->GetUserData());
        
        return user_data != util::generic_id_invalid();
      };
      
    } ray;
    
    q3RaycastData ray_data;
    ray_data.start = q3Vec3(0,-10,0);
    ray_data.dir = q3Vec3(0, 1, 0);
    ray_data.t = r32(100000.0);
    ray_data.toi = ray_data.t;
    
    world->scene.RayCast(&ray, ray_data);
  }
  
  // Collisions
  {
    Core::Collision *collisions_arr;
    uint32_t number_of_collisions = 0;
    
    Physics_transform::update_world(m_impl->world_data,
                                    &collisions_arr,
                                    &number_of_collisions);
    
    if(number_of_collisions && m_impl->collision_callback)
    {
      for(uint32_t i = 0; i < number_of_collisions; ++i)
      {
        m_impl->collision_callback(Collision_type::enter, collisions_arr[i]);
      }
    }
    
    auto to_core_trans = [](const q3Transform &other)
    {
      math::transform trans = math::transform_init_from_q3(other);
      return Core::Transform(trans.position, trans.scale, trans.rotation);
    };
  
    for(size_t i = 0; i < m_impl->world_data->physics_data->size; ++i)
    {
      if(m_impl->world_data->physics_data->property_rigidbody[i])
      {
        auto trans = reinterpret_cast<q3Body*>(m_impl->world_data->physics_data->property_rigidbody[i])->GetTransform();
        
        Core::Entity_ref ref(m_impl->world_data->physics_data->physics_id[i],
                             m_impl->world_data);
        
        auto old_tran = ref.get_transform();
        
        auto core_trans = to_core_trans(trans);
        core_trans.set_scale(old_tran.get_scale());
        
        Entity_detail::set_transform(m_impl->world_data->physics_data->physics_id[i],
                                           m_impl->world_data.get(),
                                           core_trans,
                                           false);
      }
    }
  }

  // Calculate delta_time
  {
    const util::milliseconds frame_time = m_impl->dt_timer.split();
    m_impl->dt = static_cast<float>(frame_time) / 1000.f;
    
    m_impl->running_time += m_impl->dt;
  }
  

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
  memset(cam_runs, 0, sizeof(Camera_utils::Cam_run) * cam_data->size);
  {
    number_of_cam_runs = cam_data->size;
    
    // Generate cam_run data
    {
      Core::Transform *cam_transforms = SCRATCH_ALIGNED_ALLOC(Core::Transform, cam_data->size);
      
      Camera_utils::get_camera_transforms(world->transform,
                                          cam_data->property_entity_id,
                                          cam_transforms,
                                          cam_data->size);
      
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
  const size_t draw_call_count = world->mesh_data->size;
  ::Material_renderer::Draw_call *draw_calls = SCRATCH_ALIGNED_ALLOC(::Material_renderer::Draw_call, draw_call_count);
  memset(draw_calls, 0, sizeof(::Material_renderer::Draw_call) * draw_call_count);
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
                               m_impl->context->get_width(),
                               m_impl->context->get_height(),
                               world->mesh_data,
                               resources->material_data,
                               resources->post_data,
                               cam_runs,
                               number_of_cam_runs,
                               draw_calls,
                               world->mesh_data->size,
                               &number_of_draw_calls);
  
  LOG_TODO_ONCE("Scratch code for text rendering");
  
  for(uint32_t i = 0; i < number_of_cam_runs; ++i)
  {
//    const math::mat4 scale     = math::mat4_scale(math::vec3_init(1.f));
//    const math::mat4 world     = math::mat4_multiply(math::mat4_id(), scale);
    const math::mat4 view_proj = math::mat4_multiply(cam_runs[i].view, cam_runs[i].proj);

    const GLsizei width = m_impl->context->get_width(); //cam->fbo.color_buffer[0].width; // viewport_x ?
    const GLsizei height = m_impl->context->get_height(); //cam->fbo.color_buffer[0].height; // viewport_y ?
    
    glViewport(0, 0, width, height);

    ::Text_renderer::render(view_proj, m_impl->world_data->text_data->property_draw_call, m_impl->world_data->text_data->size);
  }
  
  
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
                                        world->scene.GetBodyCount(),
                                        number_of_draw_calls,
                                        number_of_cam_runs);
  }
  #endif
}


void
World::get_overlapping_aabbs(const std::function<void(const Core::Collision_pair pairs[],
                                                      const uint32_t number_of_pairs)> &callback)
{
  LOG_TODO_ONCE("There is no need for this to be a callback like this.");

  // Check we have a callback.
  if(!callback) { return; }
  
  util::generic_id *out_ids = nullptr;
  Physics::Collision::Axis_collidable *out_axis_array = nullptr;
  size_t number_of_collidables = 0;
 
  const World_data::Physics_data *data = m_impl->world_data->physics_data;
  Transformation::get_overlapping(data->physics_id,
                                 data->property_collision_id,
                                 data->property_transformed_aabb_collider,
                                 data->property_transform,
                                 data->size,
                                 &out_axis_array,
                                 &out_ids,
                                 &number_of_collidables);

  //  
  // Calculate collisions
  Physics::Collision::Pairs out_pairs;
  Physics::Collision::pairs_init(&out_pairs, 2048 * 10);

  Physics::Collision::aabb_calculate_overlaps_pairs(&out_pairs,
                                                    out_axis_array,
                                                    number_of_collidables);
  
  
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

      pairs[number_of_pairs++] = Core::Collision_pair
      {
        find_entity_by_id(out_ids[index_a]),
        find_entity_by_id(out_ids[index_b])
      };
    }
  
    callback(pairs, number_of_pairs);
  }
  
  Physics::Collision::pairs_free(&out_pairs);
}


void
World::set_collision_callback(Collision_callback callback)
{
  assert(m_impl);
  m_impl->collision_callback = callback;
}


size_t
World::get_entity_count_in_world() const
{
  assert(m_impl && m_impl->world_data && m_impl->world_data->entity);

  return m_impl->world_data->entity->size;
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