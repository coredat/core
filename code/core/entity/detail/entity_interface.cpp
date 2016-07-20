#include <core/entity/detail/entity_interface.hpp>
#include <core/model/model.hpp>
#include <core/model/mesh.hpp>
#include <core/physics/collider.hpp>
#include <core/physics/collider_utils.hpp>
#include <core/physics/box_collider.hpp>
#include <core/physics/box_collider_utils.hpp>
#include <core/physics/rigidbody.hpp>
#include <core/resources/material.hpp>
#include <core/transform/transform.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <common/error_strings.hpp>
#include <systems/renderer_material/material.hpp>
#include <data/world_data/world_data.hpp>
#include <data/global_data/resource_data.hpp>
#include <data/world_data/transform_data.hpp>
#include <data/world_data/entity_data.hpp>
#include <data/world_data/pending_scene_graph_change_data.hpp>
#include <math/transform/transform.hpp>
#include <math/geometry/aabb.hpp>
#include <utilities/logging.hpp>
#include <utilities/bits.hpp>
#include <transformations/physics/core_to_qu3e.hpp>


namespace Core {
namespace Entity_detail {


util::generic_id
get_id(const util::generic_id this_id, World_data::World *world)
{
  return this_id;
}


bool
is_valid(const util::generic_id this_id, World_data::World *world)
{
  // Invalid data, means invalid entity.
  if(this_id == util::generic_id_invalid()) { return false; }
  if(!world)                                { return false; }

  // Our id might have expired, so we need to check it.
  data_lock(world->entity);
  const bool exists = World_data::entity_data_exists(world->entity, this_id);
  data_unlock(world->entity);
  
  return exists;
}


void
destroy(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world)) { return; }
  
  // Destroy this.
  World_data::pending_scene_graph_change_delete(world->entity_graph_changes, this_id);
}


void
set_user_data(const util::generic_id this_id, World_data::World *world, const uintptr_t user_data)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return;
  }
  
  auto entity_data = world->entity;
  
  size_t index;
  data_lock(entity_data);
  
  if(World_data::entity_data_exists(entity_data, this_id, &index))
  {
    entity_data->property_user_data[index] = user_data;
  }
  else
  {
    assert(false);
    LOG_WARNING(Error_string::entity_not_found());
  }
  
  data_unlock(entity_data);
}


uintptr_t
get_user_data(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return 0;
  }
  
  auto entity_data = world->entity;
  
  size_t index;
  uintptr_t user_data(0);
  
  data_lock(entity_data);
  
  if(World_data::entity_data_exists(entity_data, this_id, &index))
  {
    user_data = entity_data->property_user_data[index];
  }
  else
  {
    assert(false);
    LOG_WARNING(Error_string::entity_not_found());
  }
  
  data_unlock(entity_data);
  
  return user_data;
}


uint32_t
get_tags(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return 0;
  }
  
  auto entity_data = world->entity;
  
  uint32_t tags = 0;
  
  size_t index;
  data_lock(entity_data);
  
  if(World_data::entity_data_exists(entity_data, this_id, &index))
  {
    tags = entity_data->property_tag[index];
  }
  else
  {
    assert(false);
    LOG_WARNING(Error_string::entity_not_found());
  }
  
  data_unlock(entity_data);
  
  return tags;
}


bool
has_tag(const util::generic_id this_id, World_data::World *world, const uint32_t tag_id)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return 0;
  }
  
  const uint32_t tags = get_tags(this_id, world);
  
  return !!(tags & tag_id);
}


void
set_tags(const util::generic_id this_id, World_data::World *world, const uint32_t set_tags)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }
  
  auto entity_data = world->entity;
  
  size_t index;
  data_lock(entity_data);
  
  if(World_data::entity_data_exists(entity_data, this_id, &index))
  {
    entity_data->property_tag[index] = set_tags;
  }
  else
  {
    LOG_WARNING(Error_string::entity_not_found());
  }
  
  data_unlock(entity_data);
}


void
add_tag(const util::generic_id this_id, World_data::World *world, const uint32_t add_tag)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }

  const uint32_t tags = get_tags(this_id, world);
  set_tags(this_id, world, tags | add_tag);
}


void
remove_tag(const util::generic_id this_id, World_data::World *world, const uint32_t tag)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }

  const uint32_t tags = get_tags(this_id, world);
  set_tags(this_id, world, tags &~ tag);
}


void
set_name(const util::generic_id this_id, World_data::World *world, const char* set_name)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }
  
  // set string.
  World_data::data_lock(world->entity);
  
  World_data::entity_data_set_property_name(world->entity, this_id, set_name);
  
  World_data::data_unlock(world->entity);
}
  
  
const char*
get_name(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return nullptr;
  }

  const char *name;
  World_data::entity_data_get_property_name(world->entity, this_id, &name);
  
  return name;
}


namespace
{

inline void
update_transform(const util::generic_id this_id, World_data::World *world, const math::transform *transform)
{
  auto transform_data = world->transform;
  
  World_data::data_lock(transform_data);

  size_t index;
  if(World_data::transform_data_exists(transform_data, this_id, &index))
  {
    transform_data->property_transform[index] = *transform;
  }
  
  World_data::data_unlock(transform_data);
}


inline void
update_mesh_renderer(const util::generic_id this_id, World_data::World *world, const math::transform *transform)
{
  auto mesh_data = world->mesh_data;

  // Update mesh renderer data
  {
    World_data::data_lock(mesh_data);
  
    size_t mesh_index;

    if(World_data::renderer_mesh_data_exists(mesh_data, this_id, &mesh_index))
    {
      const math::mat4 world_mat = math::transform_get_world_matrix(*transform);
      memcpy(mesh_data->property_draw_call[mesh_index].world_matrix, &world_mat, sizeof(world_mat));
    }
    
    World_data::data_unlock(mesh_data);
  }
}


inline void
update_collider(const util::generic_id this_id,
                World_data::World *world,
                const math::transform *transform,
                const math::aabb *model_aabb)
{
  auto phys_data = world->physics_data;
  
  // If this is a physics object then update it.
  {
    World_data::data_lock(world->entity);
  
    uint32_t components;
    World_data::entity_data_get_property_components(world->entity,
                                                    this_id,
                                                    &components);
    
    World_data::data_unlock(world->entity);
    
    // Update the physics stuff.
    if(components & World_data::Entity_component::has_physics)
    {
      World_data::data_lock(phys_data);
      
      math::aabb collider_box;
      World_data::physics_data_get_property_aabb_collider(phys_data, this_id, &collider_box);
      
      const math::vec3 collider_scale  = math::aabb_get_extents(collider_box);
      const math::vec3 transform_scale = transform->scale;
      const math::vec3 total_scale     = math::vec3_multiply(collider_scale, transform_scale);

      // Order is important here! Scale then shift origin.
      math::aabb_scale(collider_box, total_scale);
      math::aabb_set_origin(collider_box, transform->position);      
      
      World_data::physics_data_set_property_transform(phys_data, this_id, *transform);
      World_data::physics_data_set_property_transformed_aabb_collider(phys_data, this_id, collider_box);
      
      World_data::data_unlock(phys_data);
    }
  }
}


}


void
set_transform(const util::generic_id this_id,
              World_data::World *world,
              const Transform &set_transform)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    assert(false);
    return;
  }
  
  math::aabb curr_aabb;
  World_data::data_lock(world->transform);
  World_data::transform_data_get_property_aabb(world->transform, this_id, &curr_aabb);
  World_data::data_unlock(world->transform);
  
  const math::transform new_transform = math::transform_init(set_transform.get_position(),
                                                           set_transform.get_scale(),
                                                           set_transform.get_rotation());
  
  // TODO: Some possible async ness here?
  update_transform(this_id, world, &new_transform);
  update_collider(this_id, world, &new_transform, &curr_aabb);
  update_mesh_renderer(this_id, world, &new_transform);
}


Transform
get_transform(const util::generic_id this_id, World_data::World *world)
{
  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return Transform();
  }
  
  auto data = world->transform;

  size_t index;
  if(World_data::transform_data_exists(data, this_id, &index))
  {
    math::transform local_transform = data->property_transform[index];
    
    const math::vec3 pos = local_transform.position;
    const math::vec3 scale = local_transform.scale;
    
    return Core::Transform(pos,
                           scale,
                           local_transform.rotation);
  }
  else
  {
    LOG_ERROR(Error_string::entity_not_found());
  }
  
  return Core::Transform();
}


void
set_renderer(const util::generic_id this_id,
             World_data::World *world,
             const Core::Renderer &renderer)
{
  assert(this_id && world);

  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }
  
  if(!renderer)
  {
    LOG_ERROR(Error_string::resource_is_invalid());
    return;
  }

  // Check to see if renderer has been set.
  {
    auto entity_data = world->entity;
    assert(entity_data);
    
    auto renderer_material = world->mesh_data;
    
    World_data::data_lock(entity_data);
    
    uint32_t renderer_type(0);
    World_data::entity_data_get_property_renderer(entity_data, this_id, &renderer_type);
    
    // If exists destroy old renderer
    if(renderer_type != 0)
    {
      switch((Renderer_type)renderer_type)
      {
        case(Renderer_type::material):
          World_data::data_lock(renderer_material);
          World_data::renderer_mesh_data_erase(renderer_material, this_id);
          World_data::data_unlock(renderer_material);
          break;
        
        case(Renderer_type::text):
          assert(false);
          break;
          
        default:
          break;
      }
    }
    
    World_data::data_unlock(entity_data);
  }
  
  // Set or update renderer
  switch(renderer.get_type())
  {
    case(Renderer_type::material):
    {
      const Material_renderer mat_renderer(renderer);
      set_renderer_material(this_id,
                            world,
                            mat_renderer.get_material().get_id(),
                            mat_renderer.get_model().get_id());
      break;
    }
    
    case(Renderer_type::text):
      LOG_TODO_ONCE("You have text trying to render");
      
      break;
      
    default:
      break;
  }

}


Core::Renderer
get_renderer(const util::generic_id this_id,
             World_data::World *world)
{
  assert(this_id && world);

  Core::Renderer return_renderer;

  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return return_renderer;
  }

  // Check to see if renderer has been set.
  {
    auto entity_data = world->entity;
    assert(entity_data);

    World_data::data_lock(entity_data);
    
    uint32_t renderer_type(0);
    World_data::entity_data_get_property_renderer(entity_data, this_id, &renderer_type);
    
    // What type of renderer?
    if(renderer_type != 0)
    {
      switch((Renderer_type)renderer_type)
      {
        case(Renderer_type::material):
        {
          auto renderer_material = world->mesh_data;
          assert(renderer_material);
          
          World_data::data_lock(renderer_material);
          
          util::generic_id mat_id = util::generic_id_invalid();
          World_data::renderer_mesh_data_get_property_material_id(renderer_material, this_id, &mat_id);
          
          World_data::Mesh_renderer_draw_call *draw_call = nullptr;
          World_data::renderer_mesh_data_get_property_draw_call(renderer_material, this_id, &draw_call);
          
          return_renderer = Core::Material_renderer(mat_id, draw_call->model_id);
          
          World_data::data_unlock(renderer_material);
          
          break;
        }
        
        case(Renderer_type::text):
          assert(false);
          break;
          
        default:
          break;
      }
    }
    
    World_data::data_unlock(entity_data);
  }
  
  return return_renderer;
}


void
set_renderer_material(const util::generic_id this_id,
                      World_data::World *world,
                      const util::generic_id material_id,
                      const util::generic_id model_id)
{
  assert(this_id && world);
  
  // Check to see entity has a renderer.
  // If not then set it.
  {
    auto entity_data = world->entity;
    assert(entity_data);
    
    World_data::data_lock(entity_data);
    
    uint32_t renderer_type(0);
    World_data::entity_data_get_property_renderer(entity_data, this_id, &renderer_type);
    
    if(renderer_type != 0 && (Core::Renderer_type)renderer_type != Core::Renderer_type::material)
    {
      LOG_ERROR(Error_string::no_implimentation()); // Currently no mechanism to change a renderer type.
      return;
    }
    else
    {
      World_data::entity_data_set_property_renderer(entity_data, this_id, (uint32_t)Core::Renderer_type::material);
    }
    
    World_data::data_unlock(entity_data);
  }
  
  auto mesh_data = world->mesh_data;
  assert(mesh_data);

  // We need to find the material remove it.
  // Then insert it with draw calls with the same id.
  World_data::data_lock(mesh_data);
  {
    size_t find_index;
    World_data::Mesh_renderer_draw_call *draw;
    World_data::Mesh_renderer_draw_call copy;

    // If it already exists. The data and erase the old info.
    if(World_data::renderer_mesh_data_exists(mesh_data, this_id, &find_index))
    {
      World_data::renderer_mesh_data_get_property_draw_call(mesh_data, this_id, &draw);
      copy = World_data::Mesh_renderer_draw_call(*draw);
      World_data::renderer_mesh_data_erase(mesh_data, this_id);
    }
    
    // Insert new draw call in order of material_id
    {
      size_t insert_point = mesh_data->size;
    
      const auto mat_data = Resource_data::get_resources()->material_data;
      
      Resource_data::data_lock(mat_data);
    
      ::Material_renderer::Material_id this_key;
      Resource_data::material_data_get_property_material_hash_id(mat_data, material_id, &this_key);
    
      // Loop through and find insert point
      for(size_t i = 0; i < mesh_data->size; ++i)
      {
        ::Material_renderer::Material_id other_key;
        Resource_data::material_data_get_property_material_hash_id(mat_data, mesh_data->property_material_id[i], &other_key);

        if(this_key > other_key)
        {
          insert_point = i;
          break;
        }
      }
      
      Resource_data::data_unlock(mat_data);
    
      // Get the trasnform as we are insreting a new record.
      math::transform trans;
      World_data::transform_data_get_property_transform(world->transform, this_id, &trans);
      
      const math::mat4 world_mat = math::transform_get_world_matrix(trans);
      memcpy(copy.world_matrix, &world_mat, sizeof(world_mat));
    
      World_data::renderer_mesh_data_insert(mesh_data, this_id, insert_point);
      World_data::renderer_mesh_data_set_property_material_id(mesh_data, this_id, material_id);
      World_data::renderer_mesh_data_set_property_draw_call(mesh_data, this_id, &copy);
    }
    
  }
  World_data::data_unlock(mesh_data);
  
  // Model
  {
    World_data::data_lock(mesh_data);
    
    size_t index;
    
    if(World_data::renderer_mesh_data_exists(mesh_data, this_id, &index))
    {
      mesh_data->property_draw_call[index].model_id = model_id;
    }
    else
    {
      // Has no material yet. Will insert one for the moment.
      World_data::renderer_mesh_data_insert(mesh_data, this_id, 0);
      mesh_data->property_draw_call[0].model_id = model_id;
    }
    
    World_data::data_unlock(mesh_data);
  }

  // Update aabb
  math::aabb return_aabb;
  {
    Resource_data::Mesh_data *mesh_data = Resource_data::get_resources()->mesh_data;
    assert(mesh_data);
    
    Resource_data::data_lock(mesh_data);
    Resource_data::mesh_data_get_property_aabb(mesh_data, model_id, &return_aabb);
    Resource_data::data_unlock(mesh_data);
  }
  
  auto transform_data = world->transform;
  {
    World_data::data_lock(transform_data);
    World_data::transform_data_set_property_aabb(transform_data, this_id, return_aabb);
    World_data::data_unlock(transform_data);
  }
}


void
get_renderer_material(const util::generic_id this_id,
                      World_data::World *world,
                      util::generic_id *out_material_id,
                      util::generic_id *out_model_id)
{
  auto mesh_data = world->mesh_data;

  // Material and Mesh
  {
    World_data::data_lock(mesh_data);
    
    size_t index;
    
    if(World_data::renderer_mesh_data_exists(mesh_data, this_id, &index))
    {
      *out_material_id = mesh_data->property_material_id[index];
    }
    
    if(World_data::renderer_mesh_data_exists(mesh_data, this_id, &index))
    {
      *out_model_id = mesh_data->property_draw_call[index].model_id;
    }
    
    World_data::data_unlock(mesh_data);
  }
}


namespace
{

inline void update_component(const util::generic_id this_id, World_data::World *data, const uint32_t component_id)
{
  auto entity_data = data->entity;
  
  World_data::data_lock(entity_data);

  uint32_t comps;
  World_data::entity_data_get_property_components(entity_data, this_id, &comps);

  // Add new component_id
  comps = comps | component_id;

  World_data::entity_data_set_property_components(entity_data, this_id, comps);
  
  World_data::data_unlock(entity_data);
}

}


void
set_collider(const util::generic_id this_id, World_data::World *world, const Core::Collider &collider)
{
  LOG_DEPRECATED_ONCE("set rb should do this now.");

  if(!is_valid(this_id, world))
  {
    LOG_ERROR(Error_string::entity_is_invalid());
    return;
  }
  
  auto phys_pool = world->physics_data;
  assert(phys_pool);
  
  if(phys_pool)
  {
    auto transform_data = world->transform;
    assert(transform_data);
  
    size_t index;
    if(World_data::transform_data_exists(transform_data, this_id, &index))
    {
      //TODO: This can be async
      update_component(this_id, world, World_data::Entity_component::has_physics);
    
      switch(collider.get_type())
      {
        case(Core::Collider::Type::box):
        {
          const Box_collider box_collider = Collider_utis::cast_to_box_collider(collider);
          const math::vec3 box_scale = math::vec3_init(box_collider.get_x_half_extent() * 2.f,
                                                       box_collider.get_y_half_extent() * 2.f,
                                                       box_collider.get_z_half_extent() * 2.f);
          
          math::aabb collider_box = math::aabb_init(math::vec3_init(-0.5), math::vec3_init(+0.5));
          math::aabb_scale(collider_box, box_scale);
          
          // Get the current components and add physics
          {
            math::aabb entity_aabb;
            math::transform curr_transform;
            {
              World_data::data_lock(world->transform);
              World_data::transform_data_get_property_aabb(world->transform, this_id, &entity_aabb);
              World_data::transform_data_get_property_transform(world->transform, this_id, &curr_transform);
              World_data::data_unlock(world->transform);
            }
          
            // Set physics
            {
              World_data::data_lock(phys_pool);
              
              // Add the collider box
              if(World_data::physics_data_push_back(phys_pool, this_id))
              {
                World_data::physics_data_set_property_aabb_collider(phys_pool, this_id, collider_box);
              }
              
              World_data::data_unlock(phys_pool);
              
              update_collider(this_id, world, &curr_transform, &entity_aabb);
            }
          }
          break;
        }
        
        case(Core::Collider::Type::unknown):
        default:
          assert(false);
          LOG_ERROR(Error_string::unknown_type());
      }
    }
    else
    {
      LOG_ERROR(Error_string::entity_not_found());
      return;
    }
  }
  else
  {
    LOG_ERROR(Error_string::data_not_found());
  }
}


Collider
get_collider(const util::generic_id this_id, World_data::World *world)
{
  LOG_DEPRECATED_ONCE("get rb should do this now.");
  
  assert(false);
  LOG_ERROR(Error_string::no_implimentation());
  return Collider();
}


void
set_rigidbody(const util::generic_id this_id, World_data::World *world, const Core::Rigidbody &rigidbody)
{
  LOG_TODO_ONCE("Do set collider here not in other method.")
  set_collider(this_id, world, rigidbody.get_collider());

  auto phys_pool = world->physics_data;
  assert(phys_pool);

  if (phys_pool)
  {
    // Set rb masking
    World_data::data_lock(phys_pool);
    
    const uint64_t mask = util::bits_pack(rigidbody.get_rb_id(), rigidbody.get_rb_mask());
    
    World_data::physics_data_set_property_collision_id(phys_pool, this_id, mask);
    
    World_data::data_unlock(phys_pool);
  }
  
  LOG_TODO_ONCE("This is scratch code to get rbs working")
  {
    World_data::data_lock(phys_pool);
    
    const Core::Transform transform = get_transform(this_id, world);
    
    q3Body *body;
    Physics_transform::convert_core_rb_to_qu3e(&rigidbody,
                                               &transform,
                                               &body,
                                               &world->scene,
                                               1);
    
    World_data::physics_data_set_property_rigidbody(phys_pool, this_id, body);
    
    World_data::data_unlock(phys_pool);
  }
}


Core::Rigidbody
get_rigidbody(const util::generic_id this_id, World_data::World *world)
{
  LOG_TODO_ONCE("Get rb doesn't build the rb");

  return Rigidbody();
}


} // ns
} // ns