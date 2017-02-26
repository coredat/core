#include <transformations/entity/entity_renderer.hpp>
#include <transformations/entity/entity_transform.hpp>
#include <transformations/text/rasterized_glyph_id.hpp>
#include <transformations/font/font_resource.hpp>

#include <systems/text/character.hpp>

#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/text_renderer.hpp>
#include <core/resources/material.hpp>
#include <core/model/model.hpp>
#include <core/model/mesh.hpp>
#include <core/font/font.hpp>
#include <core/world/detail/world_index.hpp>

#include <data/context_data.hpp>
#include <data/memory/memory_data.hpp>
#include <data/context/font_glyph_data.hpp>
#include <data/context/font_data.hpp>
#include <data/context/material_data.hpp>
#include <data/context/texture_data.hpp>
#include <data/context/text_mesh_data.hpp>
#include <data/world_data.hpp>
#include <data/graph/graph.hpp>
#include <data/world/mesh_draw_call_data.hpp>
#include <data/world/text_draw_call_data.hpp>
#include <data/context/material_data.hpp>
#include <data/context/mesh_data.hpp>

#include <data/graph/graph.hpp>
#include <data/renderers/text/text_renderer.hpp>

#include <common/error_strings.hpp>
#include <common/data_types.hpp>

#include <graphics_api/vertex_format.hpp>
#include <graphics_api/utils/geometry.hpp>
#include <utilities/logging.hpp>
#include <math/transform/transform.hpp>
#include <assert.h>


namespace Entity_detail {


uint32_t
has_renderer(const uint32_t this_id,
             Data::Graph::Graph_data *entity_data)
{
  uint32_t components(0);
  Data::Graph::components_get(entity_data, this_id, &components);
  
  const uint32_t renderer_type = Common::Data_type::get_renderer_type(components);
  
  return renderer_type;
}


//void
//set_renderer(const uint32_t this_id,
//             Data::Graph::Graph_data *entity_data,
//             Data::Mesh_draw_call_data *renderer_material,
//             Data::Text_draw_call_data *text_data,
//             const Core::Renderer &renderer)
//{
//  // Check valid
//  if(!is_valid(this_id, entity_data, true)) {
//    assert(false); return;
//  }
//  
//  // Renderer is valid
//  if(!renderer) {
//    LOG_ERROR(Error_string::resource_is_invalid());
//    return;
//  }
//  
//  auto world_data(Core_detail::world_index_get_world_data(1));
//  assert(world_data);
//
//  // Check to see if renderer has been set.
//  {
//    assert(entity_data);
//    assert(renderer_material);
//    
//    uint32_t components = 0;
//    Data::Graph::components_get(world_data->scene_graph, this_id, &components);
//    
//    const uint32_t renderer_type(
//      Common::Data_type::get_renderer_type(components)
//    );
//    
//    // If exists destroy old renderer
//    if(renderer_type != 0)
//    {
//      // Remove component
//      components &= ~renderer_type;
//      Data::Graph::components_set(entity_data, this_id, components);
//    
//      switch(renderer_type)
//      {
//        case(Core::Renderer_type::material):
//          Data::data_lock(renderer_material);
//          Data::mesh_draw_call_remove(renderer_material, this_id);
//          Data::data_unlock(renderer_material);
//          break;
//        
//        case(Core::Renderer_type::text):
//        // This is all wrong!
////          World_data::data_lock(text_data);
////          World_data::mesh_draw_call_erase(text_data, this_id);
////          World_data::data_unlock(text_data);
//          break;
//          
//        default:
//          break;
//      }
//    }
//  }
//  
//  // Set or update renderer
//  {
//    uint32_t components = 0;
//    Data::Graph::components_get(entity_data, this_id, &components);
//    
//    components |= renderer.get_type();
//    Data::Graph::components_set(entity_data, this_id, components);
//    
////    Data::Graph::components_set(world_data->scene_graph, this_id, components);
//    
//  }
//  
//  // Set renderer
//  switch(renderer.get_type())
//  {
//    case(Core::Renderer_type::material):
//    {
//      const Core::Material_renderer mat_renderer(renderer);
//      set_renderer_material(this_id,
//                            entity_data,
//                            renderer_material,
//                            mat_renderer.get_material().get_id(),
//                            mat_renderer.get_model().get_id());
//      break;
//    }
//    
//    case(Core::Renderer_type::text):
//    {
//      const Core::Text_renderer text_renderer(renderer);
////      set_renderer_text(this_id,
////                        "fooboo",
////                        entity_data,
////                        transform_data,
////                        text_data,
////                        text_renderer.get_font().get_id(),
////                        text_renderer.get_text_id());
//      
//      // -- New Text Renderer -- //
//      {
//        auto resources = Data::get_context_data();
//        auto world = Core_detail::world_index_get_world_data(1);
//        
//        math::transform transform;
//        Data::Graph::transform_get(entity_data, this_id, &transform);
//        math::mat4 world_mat = math::transform_get_world_matrix(transform);
//        
//        Data::Text_renderer::set_draw_call(
//          world->text_renderer,
//          this_id,
//          text_renderer.get_font().get_id(),
//          "foo\nbart",
//          math::mat4_get_data(world_mat),
//          resources->op_context,
//          resources->op_buffer
//        );
//      }
//      
//      break;
//    }
//      
//    default:
//      break;
//  }
//
//}


Core::Renderer
get_renderer(const uint32_t this_id,
             Data::Graph::Graph_data *entity_data,
             Data::Mesh_draw_call_data *renderer_material,
             Data::Text_draw_call_data *text_draw_call_data)
{
  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false);// return Core::Renderer();
  }

//  Core::Renderer return_renderer;
  
  auto world_data(Core_detail::world_index_get_world_data(1));
  assert(world_data);
  

  // Check to see if renderer has been set.
  {
    assert(entity_data);

    assert(renderer_material);
    
    Data::data_lock(renderer_material);
//    Data::data_lock(entity_data);
    
    uint32_t components = 0;
//    Data::entity_get_components(entity_data, this_id, &components);
    Data::Graph::components_get(world_data->scene_graph, this_id, &components);
    
    const uint32_t renderer_type = Common::Data_type::get_renderer_type(components);
    
    // What type of renderer?
    if(renderer_type != 0)
    {
      switch(renderer_type)
      {
        case(Core::Renderer_type::material):
        {
          uint32_t mat_id = 0;
          Data::mesh_draw_call_get_material_id(renderer_material, this_id, &mat_id);
          
          Data::Mesh_renderer_draw_call draw_call;
          Data::mesh_draw_call_get_draw_call(renderer_material, this_id, &draw_call);
          
          /*return_renderer = */
//          return Core::Material_renderer(mat_id, draw_call.model_id);

          break;
        }
        
        case(Core::Renderer_type::text):
        {
//          uint32_t font_id = 0;
          
          ::Text_renderer::Draw_call dc;
          
//          return_renderer = Core::Text_renderer();
          
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
    
    Data::data_unlock(renderer_material);
  }
  
//  return return_renderer;
  return Core::Renderer();
}


void
set_renderer_material(const uint32_t this_id,
                      Data::Graph::Graph_data *entity_data,
                      Data::Mesh_draw_call_data *mesh_data,
                      const uint32_t material_id,
                      const uint32_t model_id)
{
  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }
  
  assert(mesh_data);

  // We need to find the material remove it.
  // Then insert it with draw calls with the same id.
  {
    const auto mat_data = Data::get_context_data()->material_data;
    assert(mat_data);
  
    Data::data_lock(mesh_data);
    Data::data_lock(mat_data);
  
    size_t find_index;
    Data::Mesh_renderer_draw_call draw;
    Data::Mesh_renderer_draw_call copy;

    // If it already exists. The data and erase the old info.
    if(Data::mesh_draw_call_exists(mesh_data, this_id, &find_index))
    {
      Data::mesh_draw_call_get_draw_call(mesh_data, this_id, &draw);
      copy = Data::Mesh_renderer_draw_call(draw);
      Data::mesh_draw_call_remove(mesh_data, this_id);
    }
    
    // Insert new draw call in order of material_id
    {
      size_t insert_point = 0;
    
      ::Material_renderer::Material_id this_key;
      Data::material_get_material_hash(mat_data, material_id, &this_key);
    
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
      Data::Graph::transform_get(entity_data, this_id, &trans);
      
      const math::mat4 world_mat = math::transform_get_world_matrix(trans);
      memcpy(copy.world_matrix, &world_mat, sizeof(world_mat));
    
      Data::mesh_draw_call_insert(mesh_data, this_id, insert_point);
      Data::mesh_draw_call_set_material_id(mesh_data, this_id, &material_id);
      Data::mesh_draw_call_set_draw_call(mesh_data, this_id, &copy);
    }
    
    Data::data_unlock(mat_data);
    Data::data_unlock(mesh_data);
  }
  
  // Model
  {
    Data::data_lock(mesh_data);
    
    size_t index;
    
    if(Data::mesh_draw_call_exists(mesh_data, this_id, &index))
    {
      mesh_data->field_draw_call[index].model_id = model_id;
    }
    else
    {
      // Has no material yet. Will insert one for the moment.
      Data::mesh_draw_call_insert(mesh_data, this_id, 0);
      mesh_data->field_draw_call[0].model_id = model_id;
    }
    
    Data::data_unlock(mesh_data);
  }

  // Update aabb
  math::aabb return_aabb;
  {
    Data::Mesh_data *mesh_data = Data::get_context_data()->mesh_data;
    assert(mesh_data);
    
    Data::data_lock(mesh_data);
    Data::mesh_get_aabb(mesh_data, model_id, &return_aabb);
    Data::data_unlock(mesh_data);
  }
  
  {
    Data::Graph::aabb_set(entity_data, this_id, return_aabb);
  }
}


void
get_renderer_material(const uint32_t this_id,
                      Data::Mesh_draw_call_data *mesh_data,
                      uint32_t *out_material_id,
                      uint32_t *out_model_id)
{
  // Material and Mesh
  {
    Data::data_lock(mesh_data);
    
    size_t index;
    
    if(Data::mesh_draw_call_exists(mesh_data, this_id, &index))
    {
      *out_material_id = mesh_data->field_material_id[index];
    }
    
    if(Data::mesh_draw_call_exists(mesh_data, this_id, &index))
    {
      *out_model_id = mesh_data->field_draw_call[index].model_id;
    }
    
    Data::data_unlock(mesh_data);
  }
}


void
set_renderer_text(const uint32_t this_id,
                  const char *str,
                  Data::Graph::Graph_data *entity_data,
                  Data::Text_draw_call_data *text_data,
                  const uint32_t font_id,
                  const uint32_t model_id)
{
  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }
  
  LOG_TODO_ONCE("Are all these resources required? - locking up a large portions of data!");
  
  auto resources = Data::get_context_data();
  assert(resources);
  
  Data::Font_data       *font_data    = resources->font_data;
  Data::Texture_data    *texture_data = resources->texture_data;
  Data::Font_glyph_data *glyph_data   = resources->font_glyph_data;
  
  assert(font_data);
  assert(texture_data);
  assert(glyph_data);
  
  // Find and add missing glyphs
  Font_resource::add_glyphs(
    str,
    strlen(str),
    font_id,
    font_data,
    glyph_data,
    texture_data,
    resources->op_context,
    resources->op_buffer
  );
  
  Font_resource::create_string_data(
    this_id,
    str,
    strlen(str),
    font_id,
    font_data,
    glyph_data,
    texture_data,
    text_data,
    nullptr,
    entity_data,
    resources->op_context,
    resources->op_buffer
  );
}


} // ns
