#include <core/entity/entity_components.hpp>
#include <core/entity/entity_ref.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <core/camera/camera.hpp>
#include <core/lighting/point_light.hpp>
#include <core/color/color.hpp>
#include <core/world/detail/world_index.hpp>
#include <core/transform/transform.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/text_renderer.hpp>
#include <core/font/font.hpp>
#include <data/world/camera_data.hpp>
#include <data/context_data.hpp>
#include <data/context/mesh_data.hpp>
#include <data/renderers/text/text_renderer.hpp>
#include <data/graph/graph.hpp>
#include <data/context/material_data.hpp>
#include <data/world/mesh_draw_call_data.hpp>
#include <data/context/text_mesh_data.hpp>
#include <data/physics/physics.hpp>
#include <data/physics/config_rigidbody.hpp>
#include <data/physics/config_trigger.hpp>
#include <data/physics/config_collider.hpp>
#include <common/data_types.hpp>
#include <common/error_strings.hpp>
#include <transformations/entity/entity_camera.hpp>
#include <transformations/entity/entity_transform.hpp>
#include <transformations/entity/entity_renderer.hpp>
#include <transformations/entity/entity_rigidbody.hpp>
#include <transformations/entity/entity_light.hpp>
#include <utilities/utilities.hpp>
#include <math/math.hpp>


namespace Core {
namespace Entity_component {


// ------------------------------------------------- [ Transform Component ] --

bool
set_transform(const Core::Entity_ref &ref,
              const Core::Transform &transform)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return false;
  }

  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
  // Update Scene Graph
  {
    math::transform trans;
    trans.rotation = transform.get_rotation();
    trans.scale    = transform.get_scale();
    trans.position = transform.get_position();
    Data::Graph::transform_set(world_data->scene_graph, entity_uint_id, trans);
  }
  
  // Old stuff
  Entity_detail::set_transform(entity_uint_id,
                               world_data->scene_graph,
                               world_data->rigidbody_data,
                               nullptr,
                               world_data->trigger_data,
                               world_data->mesh_data,
                               world_data->text_data,
                               transform);
  
  return true;
}


Core::Transform
get_transform(const Core::Entity_ref &ref)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return Core::Transform();
  }

  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
  // Get Scene Graph
  math::transform trans;
  Data::Graph::transform_get(world_data->scene_graph, entity_uint_id, &trans);

  return Core::Transform(trans.position, trans.scale, trans.rotation);
}


bool
has_transform(const Core::Entity_ref &ref)
{
  // You cannot remove a transform from an entity.
  return true;
}


// -------------------------------------------------- [ Renderer Component ] --

bool
set_renderer(const Core::Entity_ref &ref,
             const Core::Material_renderer &renderer)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return false;
  }

  const uint32_t entity_uint_id(ref.get_id());
  
  const Core_detail::Entity_id entity_id(
    Core_detail::entity_id_from_uint(entity_uint_id)
  );

  auto world_data(
    Core_detail::world_index_get_world_data(entity_id.world_instance)
  );
  assert(world_data);
  
  auto entity_data = world_data->scene_graph;
  
  auto mesh_data = world_data->mesh_data;
  
  auto renderer_material = world_data->mesh_data;

  const auto mat_data = Data::get_context_data()->material_data;
  assert(mat_data);
  
  uint32_t components = 0;
  Data::Graph::components_get(
    world_data->scene_graph, ref.get_id(), &components
  );
  
  const uint32_t renderer_type(
    Common::Data_type::get_renderer_type(components)
  );
  
  if(renderer_type == 0)
  {
    const auto mat_data = Data::get_context_data()->material_data;
    assert(mat_data);
  
    Data::data_lock(mesh_data);
    Data::data_lock(mat_data);
  
    size_t find_index;
    Data::Mesh_renderer_draw_call draw;
    Data::Mesh_renderer_draw_call copy;

    // If it already exists. The data and erase the old info.
    if(Data::mesh_draw_call_exists(mesh_data, ref.get_id(), &find_index))
    {
      Data::mesh_draw_call_get_draw_call(mesh_data, ref.get_id(), &draw);
      copy = Data::Mesh_renderer_draw_call(draw);
      Data::mesh_draw_call_remove(mesh_data, ref.get_id());
    }
    
    // Insert new draw call in order of material_id
    {
      size_t insert_point = 0;
    
      ::Material_renderer::Material_id this_key;
      Data::material_get_material_hash(mat_data, renderer.get_material_id(), &this_key);
    
      // Loop through and find insert point
      for(size_t i = 0; i < mesh_data->size; ++i)
      {
        ::Material_renderer::Material_id other_key;
        Data::material_get_material_hash(mat_data, mesh_data->field_material_id[i], &other_key);

        if(this_key > other_key)
        {
          insert_point = i;
          break;
        }
      }
      
      // Get the trasnform as we are insreting a new record.
      math::transform trans;
      Data::Graph::transform_get(entity_data, ref.get_id(), &trans);
      
      const math::mat4 world_mat = math::transform_get_world_matrix(trans);
      memcpy(copy.world_matrix, &world_mat, sizeof(world_mat));
    
      Data::mesh_draw_call_insert(mesh_data, ref.get_id(), insert_point);
      
      auto mat_id = renderer.get_material_id();
      
      Data::mesh_draw_call_set_material_id(mesh_data, ref.get_id(), &mat_id);
      Data::mesh_draw_call_set_draw_call(mesh_data, ref.get_id(), &copy);
    }
    
    Data::data_unlock(mat_data);
    Data::data_unlock(mesh_data);
  }
  
  // Model
  {
    Data::data_lock(mesh_data);
    
    size_t index;
    
    if(Data::mesh_draw_call_exists(mesh_data, ref.get_id(), &index))
    {
      mesh_data->field_draw_call[index].model_id = renderer.get_model_id();
    }
    else
    {
      // Has no material yet. Will insert one for the moment.
      Data::mesh_draw_call_insert(mesh_data, ref.get_id(), 0);
      mesh_data->field_draw_call[0].model_id = renderer.get_model_id();
    }
    
    Data::data_unlock(mesh_data);
  }

  // Update aabb
  math::aabb return_aabb;
  {
    Data::Mesh_data *mesh_data2 = Data::get_context_data()->mesh_data;
    assert(mesh_data2);
    
    Data::data_lock(mesh_data2);
    Data::mesh_get_aabb(mesh_data2, renderer.get_model_id(), &return_aabb);
    Data::data_unlock(mesh_data2);
  }
  
  {
    Data::Graph::aabb_set(entity_data, ref.get_id(), return_aabb);
  }

  
//  Entity_detail::set_renderer(entity_uint_id,
//                              world_data->scene_graph,
//                              world_data->mesh_data,
//                              world_data->text_data,
//                              renderer);
  
  return true;
}


bool
set_renderer(const Core::Entity_ref &ref,
             const Core::Text_renderer &text_renderer)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return false;
  }
  
  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
  // -- Check to see if renderer is attached -- //
  {
    uint32_t components = 0;
    Data::Graph::components_get(world_data->scene_graph, entity_uint_id, &components);
    
    const uint32_t renderer_type = Common::Data_type::get_renderer_type(components);
  
    if(renderer_type != 0)
    {
      return false;
    }
    else
    {
      // Remove component
      components &= Common::Data_type::renderer_text;
      Data::Graph::components_set(world_data->scene_graph, entity_uint_id, components);
    }
  }

  // -- New Text Renderer -- //
  {
    auto resources = Data::get_context_data();
    auto world = Core_detail::world_index_get_world_data(1);
    
    math::transform transform;
    Data::Graph::transform_get(world->scene_graph, entity_uint_id, &transform);
    math::mat4 world_mat = math::transform_get_world_matrix(transform);
    
    Data::Text_renderer::set_draw_call(
      world->text_renderer,
      entity_uint_id,
      text_renderer.get_font().get_id(),
      text_renderer.get_text(),
      math::mat4_get_data(world_mat),
      resources->op_context,
      resources->op_buffer
    );
  }

  
  return true;
}
  

Core::Renderer
get_renderer(const Core::Entity_ref &ref)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return Core::Renderer();
  }

  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
  return Entity_detail::get_renderer(entity_uint_id,
                                     world_data->scene_graph,
                                     world_data->mesh_data,
                                     world_data->text_data);
}


void
remove_renderer(const Core::Entity_ref &renderer)
{
  assert(false); // not impl because churn going on here.
}


bool
has_renderer(const Core::Entity_ref &ref)
{
  assert(false);
  return false;
}


bool
has_text_renderer(const Core::Entity_ref &ref)
{
  assert(false);
  return false;
}


// ------------------------------------------------- [ Rigidbody Component ] --

bool
set_rigidbody(const Core::Entity_ref &ref,
              const Core::Rigidbody &rigidbody)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return false;
  }
  
  const uint32_t this_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(this_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
  // -- Get Data -- //
  Data::Graph::Graph_data *graph = world_data->scene_graph;
  LIB_ASSERT(graph);
  
  Data::Physics::Physics_data *phys = world_data->physics;
  LIB_ASSERT(phys);
  
  // -- Check/Set Components -- //
  uint32_t comps = 0;
  {
    Data::Graph::components_get(graph, this_id, &comps);

    // Triggers and RB's are treated differently.
    if(Common::Data_type::is_collidable(comps))
    {
      // Remove if there is a discrepency.
      if(rigidbody.is_trigger() && (comps & Common::Data_type::rigidbody))
      {
        Data::Physics::rigidbody_remove(phys, this_id);
        comps &= ~(Common::Data_type::rigidbody);
      }
      else if(!rigidbody.is_trigger() && (comps & Common::Data_type::trigger))
      {
        Data::Physics::trigger_remove(phys, this_id);
        comps &= ~(Common::Data_type::trigger);
      }
    }
  }
  
  // -- Get Transforms and Set RB -- //
  math::transform trans;
  Data::Graph::transform_get(graph, this_id, &trans);
  
  math::aabb aabb;
  Data::Graph::aabb_get(graph, this_id, &aabb);
  
  // -- Build RB and Collider -- //
  const Core::Collider core_coll = rigidbody.get_collider();
  
  Data::Physics::Collider_config coll;
  coll.type = core_coll.get_type() == Core::Collider::Type::box ?  Data::Physics::Collider_type::box : Data::Physics::Collider_type::unknown;
  coll.args[0] = core_coll.get_arg_01();
  coll.args[1] = core_coll.get_arg_02();
  coll.args[2] = core_coll.get_arg_03();
  
  if(!rigidbody.is_trigger())
  {
    Data::Physics::Rigidbody_config rb;
    rb.mass = rigidbody.get_mass();
    rb.is_kinematic = rigidbody.is_kinematic();
    
    if(comps & Common::Data_type::rigidbody)
    {
      Data::Physics::rigidbody_update(phys, this_id, &rb, &coll, &trans, &aabb);
    }
    else
    {
      Data::Physics::rigidbody_add(phys, this_id, &rb, &coll, &trans, &aabb);
      
      comps |= Common::Data_type::rigidbody;
      Data::Graph::components_set(graph, this_id, comps);
    }
  }
  else
  {
    Data::Physics::Trigger_config trig;
    
    if(comps & Common::Data_type::trigger)
    {
      Data::Physics::trigger_udpate(phys, this_id, &trig, &coll, &trans, &aabb);
    }
    else
    {
      Data::Physics::trigger_add(phys, this_id, &trig, &coll, &trans, &aabb);
      
      comps |= Common::Data_type::trigger;
      Data::Graph::components_set(graph, this_id, comps);
    }
  }
  
  return true;
}
  

Core::Rigidbody
get_rigidbody(const Core::Entity_ref &ref)
{
  return Core::Rigidbody();

//  if(!ref)
//  {
//    LOG_ERROR(Error_string::entity_is_invalid());
//    assert(false);
//    return Core::Rigidbody();
//  }
//
//  const uint32_t entity_uint_id(ref.get_id());
//  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);
//
//  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
//  assert(world_data);
//  
//  return Entity_detail::get_rigidbody(
//    entity_uint_id,
//    world_data->scene_graph,
//    world_data->rigidbody_data,
//    world_data->trigger_data
//  );
}


void
remove_rigidbody(const Core::Entity_ref &rigidbody)
{
  assert(false); // churn alert
}


bool
has_rigidbody(const Core::Entity_ref &ref)
{
  assert(false); // churn alert.
  return false;
}


// ---------------------------------------------------- [ Camera Component ] --


bool
set_camera(const Core::Entity_ref &ref,
           const Core::Camera &camera)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return false;
  }

  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
  Data::Camera_data *camera_data = world_data->camera_data;
  assert(camera_data);

  Entity_detail::set_camera(entity_uint_id, world_data->scene_graph, camera_data, &camera);
  
  return true;
}


Core::Camera
get_camera(const Core::Entity_ref &ref)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return Core::Camera();
  }
  
  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  Core::Camera return_camera;

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);

  Data::Camera_data *cam_data(world_data->camera_data);
  assert(cam_data);
  
  Entity_detail::get_camera(entity_uint_id, cam_data, &return_camera);
  
  return return_camera;
}


void
remove_camera(const Core::Entity_ref &ref)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return;
  }
  
  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(ref.get_id());
  
  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
//  Data::Entity_data *entity_data = world_data->entity;
//  assert(entity_data);

  Data::Camera_data *cam_data(world_data->camera_data);
  assert(cam_data);
  
  Entity_detail::remove_camera(entity_uint_id, nullptr, cam_data);
}


bool
has_camera(const Core::Entity_ref &ref)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return false;
  }

  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);

  Data::Camera_data *cam_data(world_data->camera_data);
  assert(cam_data);
  
  return Entity_detail::has_camera(entity_uint_id, cam_data);
}


// ----------------------------------------------------- [ Light Component ] --

bool
set_light(const Core::Entity_ref &ref,
          const Core::Point_light &light)
{
  if(!ref)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return false;
  }
  
  const uint32_t entity_uint_id(ref.get_id());
  const Core_detail::Entity_id entity_id = Core_detail::entity_id_from_uint(entity_uint_id);

  auto world_data(Core_detail::world_index_get_world_data(entity_id.world_instance));
  assert(world_data);
  
//  Data::Entity_data *entity_data = world_data->entity;
//  assert(entity_data);
  
  Data::Light_data *light_data = world_data->light_data;
  assert(light_data);
  
//  Data::Transform_data *transform_data = world_data->transform;
//  assert(transform_data);

  Entity_detail::set_light(entity_uint_id, world_data->scene_graph, light_data, &light);
  
  return true;
}
  
  
Core::Point_light
get_light(const Core::Entity_ref &ref)
{
  return Core::Point_light();
}


void
remove_light(const Core::Entity_ref &ref)
{
}


bool
has_light(const Core::Entity_ref &ref)
{
  return false;
}


} // ns
} // ns
