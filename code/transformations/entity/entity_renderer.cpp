#include <transformations/entity/entity_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/text_renderer.hpp>
#include <core/resources/material.hpp>
#include <core/model/model.hpp>
#include <core/model/mesh.hpp>
#include <data/global_data/resource_data.hpp>
#include <data/world_data/world_data.hpp>
#include <data/world_data/entity_data.hpp>
#include <data/world_data/transform_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace Entity_detail {


void
set_renderer(const util::generic_id this_id,
             World_data::World *world,
             const Core::Renderer &renderer)
{
  // Check valid
  if(!is_valid(this_id, world, true)) {
    assert(false); return;
  }
  
  // Renderer is valid
  if(!renderer) {
    LOG_ERROR(Error_string::resource_is_invalid());
    return;
  }

  // Check to see if renderer has been set.
  {
    auto entity_data = world->entity;
    assert(entity_data);
    
    auto renderer_material = world->mesh_data;
    assert(renderer_material);
    
    World_data::data_lock(entity_data);
    World_data::data_lock(renderer_material);
    
    uint32_t renderer_type = 0;
    World_data::entity_data_get_property_renderer(entity_data, this_id, &renderer_type);
    
    // If exists destroy old renderer
    if(renderer_type != 0)
    {
      switch((Core::Renderer_type)renderer_type)
      {
        case(Core::Renderer_type::material):
          World_data::renderer_mesh_data_erase(renderer_material, this_id);
          break;
        
        case(Core::Renderer_type::text):
          World_data::renderer_mesh_data_erase(renderer_material, this_id);
          break;
          
        default:
          break;
      }
    }
    
    World_data::data_unlock(renderer_material);
    World_data::data_unlock(entity_data);
  }
  
  // Set or update renderer
  switch(renderer.get_type())
  {
    case(Core::Renderer_type::material):
    {
      const Core::Material_renderer mat_renderer(renderer);
      set_renderer_material(this_id,
                            world,
                            mat_renderer.get_material().get_id(),
                            mat_renderer.get_model().get_id());
      break;
    }
    
    case(Core::Renderer_type::text):
    {
      const Core::Text_renderer tex_renderer(renderer);
      LOG_TODO_ONCE("You have text trying to render");
      
      break;
    }
      
    default:
      break;
  }

}


Core::Renderer
get_renderer(const util::generic_id this_id,
             World_data::World *world)
{
  // Check valid
  if(!is_valid(this_id, world, true)) {
    assert(false); return Core::Renderer();
  }

  Core::Renderer return_renderer;

  // Check to see if renderer has been set.
  {
    auto entity_data = world->entity;
    assert(entity_data);

    auto renderer_material = world->mesh_data;
    assert(renderer_material);
    
    World_data::data_lock(renderer_material);
    World_data::data_lock(entity_data);
    
    uint32_t renderer_type = 0;
    World_data::entity_data_get_property_renderer(entity_data, this_id, &renderer_type);
    
    // What type of renderer?
    if(renderer_type != 0)
    {
      switch((Core::Renderer_type)renderer_type)
      {
        case(Core::Renderer_type::material):
        {
          util::generic_id mat_id = util::generic_id_invalid();
          World_data::renderer_mesh_data_get_property_material_id(renderer_material, this_id, &mat_id);
          
          World_data::Mesh_renderer_draw_call *draw_call = nullptr;
          World_data::renderer_mesh_data_get_property_draw_call(renderer_material, this_id, &draw_call);
          
          return_renderer = Core::Material_renderer(mat_id, draw_call->model_id);

          break;
        }
        
        case(Core::Renderer_type::text):
        {
          assert(false);
          break;
        }
          
        default:
        {
          assert(false);
          LOG_ERROR(Error_string::data_not_found());
          break;
        }
      }
    }
    
    World_data::data_unlock(renderer_material);
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
  // Check valid
  if(!is_valid(this_id, world, true)) {
    assert(false); return;
  }
  
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
  {
    const auto mat_data = Resource_data::get_resources()->material_data;
    assert(mat_data);
  
    World_data::data_lock(mesh_data);
    Resource_data::data_lock(mat_data);
  
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
      
      // Get the trasnform as we are insreting a new record.
      math::transform trans;
      World_data::transform_data_get_property_transform(world->transform, this_id, &trans);
      
      const math::mat4 world_mat = math::transform_get_world_matrix(trans);
      memcpy(copy.world_matrix, &world_mat, sizeof(world_mat));
    
      World_data::renderer_mesh_data_insert(mesh_data, this_id, insert_point);
      World_data::renderer_mesh_data_set_property_material_id(mesh_data, this_id, material_id);
      World_data::renderer_mesh_data_set_property_draw_call(mesh_data, this_id, &copy);
    }
    
    Resource_data::data_unlock(mat_data);
    World_data::data_unlock(mesh_data);
  }
  
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


void
set_renderer_text(const util::generic_id this_id,
                  World_data::World *world,
                  const util::generic_id font_id,
                  const util::generic_id model_id)
{
//  assert(this_id && world);
//  
//  // Check to see entity has a renderer.
//  // If not then set it.
//  {
//    auto entity_data = world->entity;
//    assert(entity_data);
//    
//    World_data::data_lock(entity_data);
//    
//    uint32_t renderer_type(0);
//    World_data::entity_data_get_property_renderer(entity_data, this_id, &renderer_type);
//    
//    if(renderer_type != 0 && (Core::Renderer_type)renderer_type != Core::Renderer_type::text)
//    {
//      LOG_ERROR(Error_string::no_implimentation()); // Currently no mechanism to change a renderer type.
//      return;
//    }
//    else
//    {
//      World_data::entity_data_set_property_renderer(entity_data, this_id, (uint32_t)Core::Renderer_type::material);
//    }
//    
//    World_data::data_unlock(entity_data);
//  }
//  
//  auto mesh_data = world->mesh_data;
//  assert(mesh_data);
//
//  // We need to find the material remove it.
//  // Then insert it with draw calls with the same id.
//  World_data::data_lock(mesh_data);
//  {
//    size_t find_index;
//    World_data::Mesh_renderer_draw_call *draw;
//    World_data::Mesh_renderer_draw_call copy;
//
//    // If it already exists. The data and erase the old info.
//    if(World_data::renderer_mesh_data_exists(mesh_data, this_id, &find_index))
//    {
//      World_data::renderer_mesh_data_get_property_draw_call(mesh_data, this_id, &draw);
//      copy = World_data::Mesh_renderer_draw_call(*draw);
//      World_data::renderer_mesh_data_erase(mesh_data, this_id);
//    }
//    
//    // Insert new draw call in order of material_id
//    {
//      size_t insert_point = mesh_data->size;
//    
//      const auto mat_data = Resource_data::get_resources()->material_data;
//      
//      Resource_data::data_lock(mat_data);
//    
//      ::Material_renderer::Material_id this_key;
//      Resource_data::material_data_get_property_material_hash_id(mat_data, material_id, &this_key);
//    
//      // Loop through and find insert point
//      for(size_t i = 0; i < mesh_data->size; ++i)
//      {
//        ::Material_renderer::Material_id other_key;
//        Resource_data::material_data_get_property_material_hash_id(mat_data, mesh_data->property_material_id[i], &other_key);
//
//        if(this_key > other_key)
//        {
//          insert_point = i;
//          break;
//        }
//      }
//      
//      Resource_data::data_unlock(mat_data);
//    
//      // Get the trasnform as we are insreting a new record.
//      math::transform trans;
//      World_data::transform_data_get_property_transform(world->transform, this_id, &trans);
//      
//      const math::mat4 world_mat = math::transform_get_world_matrix(trans);
//      memcpy(copy.world_matrix, &world_mat, sizeof(world_mat));
//    
//      World_data::renderer_mesh_data_insert(mesh_data, this_id, insert_point);
//      World_data::renderer_mesh_data_set_property_material_id(mesh_data, this_id, material_id);
//      World_data::renderer_mesh_data_set_property_draw_call(mesh_data, this_id, &copy);
//    }
//    
//  }
//  World_data::data_unlock(mesh_data);
//  
//  // Model
//  {
//    World_data::data_lock(mesh_data);
//    
//    size_t index;
//    
//    if(World_data::renderer_mesh_data_exists(mesh_data, this_id, &index))
//    {
//      mesh_data->property_draw_call[index].model_id = model_id;
//    }
//    else
//    {
//      // Has no material yet. Will insert one for the moment.
//      World_data::renderer_mesh_data_insert(mesh_data, this_id, 0);
//      mesh_data->property_draw_call[0].model_id = model_id;
//    }
//    
//    World_data::data_unlock(mesh_data);
//  }
//
//  // Update aabb
//  math::aabb return_aabb;
//  {
//    Resource_data::Mesh_data *mesh_data = Resource_data::get_resources()->mesh_data;
//    assert(mesh_data);
//    
//    Resource_data::data_lock(mesh_data);
//    Resource_data::mesh_data_get_property_aabb(mesh_data, model_id, &return_aabb);
//    Resource_data::data_unlock(mesh_data);
//  }
//  
//  auto transform_data = world->transform;
//  {
//    World_data::data_lock(transform_data);
//    World_data::transform_data_set_property_aabb(transform_data, this_id, return_aabb);
//    World_data::data_unlock(transform_data);
//  }
}


} // ns