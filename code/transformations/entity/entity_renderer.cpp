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

#include <data/context_data.hpp>
#include <data/memory/memory_data.hpp>
#include <data/context/font_glyph_data.hpp>
#include <data/context/font_data.hpp>
#include <data/context/material_data.hpp>
#include <data/context/texture_data.hpp>
#include <data/context/text_mesh_data.hpp>
#include <data/world_data.hpp>
#include <data/world/entity_data.hpp>
#include <data/world/transform_data.hpp>
#include <data/world/mesh_draw_call_data.hpp>
#include <data/world/text_draw_call_data.hpp>
#include <data/context/material_data.hpp>
#include <data/context/mesh_data.hpp>

#include <common/error_strings.hpp>
#include <common/data_types.hpp>

#include <graphics_api/vertex_format.hpp>
#include <graphics_api/utils/geometry.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace Entity_detail {


uint32_t
has_renderer(const util::generic_id this_id,
             Data::Entity_data *entity_data)
{
  Data::data_lock(entity_data);
  
  uint32_t components(0);
  Data::entity_get_components(entity_data, this_id, &components);
  
  const uint32_t renderer_type = Common::Data_type::get_renderer_type(components);
  
  Data::data_unlock(entity_data);

  return renderer_type;
}


void
set_renderer(const util::generic_id this_id,
             Data::Entity_data *entity_data,
             Data::Transform_data *transform_data,
             Data::Mesh_draw_call_data *renderer_material,
             Data::Text_draw_call_data *text_data,
             const Core::Renderer &renderer)
{
  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }
  
  // Renderer is valid
  if(!renderer) {
    LOG_ERROR(Error_string::resource_is_invalid());
    return;
  }

  // Check to see if renderer has been set.
  {
    assert(entity_data);
    assert(renderer_material);
    
    Data::data_lock(entity_data);
    
    uint32_t components = 0;
    Data::entity_get_components(entity_data, this_id, &components);
    
    const uint32_t renderer_type = Common::Data_type::get_renderer_type(components);
    
    // If exists destroy old renderer
    if(renderer_type != 0)
    {
      // Remove component
      components &= ~renderer_type;
      Data::entity_set_components(entity_data, this_id, &components);
    
      switch(renderer_type)
      {
        case(Core::Renderer_type::material):
          Data::data_lock(renderer_material);
          Data::mesh_draw_call_remove(renderer_material, this_id);
          Data::data_unlock(renderer_material);
          break;
        
        case(Core::Renderer_type::text):
        // This is all wrong!
//          World_data::data_lock(text_data);
//          World_data::mesh_draw_call_erase(text_data, this_id);
//          World_data::data_unlock(text_data);
          break;
          
        default:
          break;
      }
    }
    
    Data::data_unlock(entity_data);
  }
  
  // Set or update renderer
  {
    Data::data_lock(entity_data);
    
    uint32_t components = 0;
    Data::entity_get_components(entity_data, this_id, &components);
    
    components |= renderer.get_type();
    Data::entity_set_components(entity_data, this_id, &components);
    
    Data::data_unlock(entity_data);
  }
  
  // Set renderer
  switch(renderer.get_type())
  {
    case(Core::Renderer_type::material):
    {
      const Core::Material_renderer mat_renderer(renderer);
      set_renderer_material(this_id,
                            entity_data,
                            transform_data,
                            renderer_material,
                            mat_renderer.get_material().get_id(),
                            mat_renderer.get_model().get_id());
      break;
    }
    
    case(Core::Renderer_type::text):
    {
      const Core::Text_renderer text_renderer(renderer);
      set_renderer_text(this_id,
                        entity_data,
                        transform_data,
                        text_data,
                        text_renderer.get_font_id(),
                        text_renderer.get_text_id());
      
      break;
    }
      
    default:
      break;
  }

}


Core::Renderer
get_renderer(const util::generic_id this_id,
             Data::Entity_data *entity_data,
             Data::Mesh_draw_call_data *renderer_material,
             Data::Text_draw_call_data *text_draw_call_data)
{
  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return Core::Renderer();
  }

  Core::Renderer return_renderer;

  // Check to see if renderer has been set.
  {
    assert(entity_data);

    assert(renderer_material);
    
    Data::data_lock(renderer_material);
    Data::data_lock(entity_data);
    
    uint32_t components = 0;
    Data::entity_get_components(entity_data, this_id, &components);
    
    const uint32_t renderer_type = Common::Data_type::get_renderer_type(components);
    
    // What type of renderer?
    if(renderer_type != 0)
    {
      switch(renderer_type)
      {
        case(Core::Renderer_type::material):
        {
          util::generic_id mat_id = util::generic_id_invalid();
          Data::mesh_draw_call_get_material_id(renderer_material, this_id, &mat_id);
          
          Data::Mesh_renderer_draw_call draw_call;
          Data::mesh_draw_call_get_draw_call(renderer_material, this_id, &draw_call);
          
          return_renderer = Core::Material_renderer(mat_id, draw_call.model_id);

          break;
        }
        
        case(Core::Renderer_type::text):
        {
          util::generic_id font_id = util::generic_id_invalid();
          
          return_renderer = Core::Text_renderer();
          
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
    Data::data_unlock(entity_data);
  }
  
  return return_renderer;
}


void
set_renderer_material(const util::generic_id this_id,
                      Data::Entity_data *entity_data,
                      Data::Transform_data *transform_data,
                      Data::Mesh_draw_call_data *mesh_data,
                      const util::generic_id material_id,
                      const util::generic_id model_id)
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
    Data::Mesh_renderer_draw_call *draw = nullptr;
    Data::Mesh_renderer_draw_call copy;

    // If it already exists. The data and erase the old info.
    if(Data::mesh_draw_call_exists(mesh_data, this_id, &find_index))
    {
      Data::mesh_draw_call_get_draw_call(mesh_data, this_id, draw);
      copy = Data::Mesh_renderer_draw_call(*draw);
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
      Data::transform_get_transform(transform_data, this_id, &trans);
      
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
    Data::data_lock(transform_data);
    Data::transform_set_aabb(transform_data, this_id, &return_aabb);
    Data::data_unlock(transform_data);
  }
}


void
get_renderer_material(const util::generic_id this_id,
                      Data::Mesh_draw_call_data *mesh_data,
                      util::generic_id *out_material_id,
                      util::generic_id *out_model_id)
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
set_renderer_text(const util::generic_id this_id,
                  Data::Entity_data *entity_data,
                  Data::Transform_data *transform_data,
                  Data::Text_draw_call_data *text_data,
                  const util::generic_id font_id,
                  const util::generic_id model_id)
{
  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }
  
  LOG_TODO_ONCE("Are all these resources required? - locking up a large portion of data!");
  
  auto resources = Data::get_context_data();
  assert(resources);
  
  auto text_mesh_data = resources->text_mesh_data;
  assert(text_mesh_data);
  
  // Set the renderer and build the mesh
  auto font_data = resources->font_data;
  assert(font_data);
  
  auto texture_data = resources->texture_data;
  assert(texture_data);
  
  auto glyph_data = resources->font_glyph_data;
  assert(glyph_data);
  
  const char * str = "foobar!!";
  
  // Find and add missing glyphs
  Font_resource::add_glyphs(str, strlen(str), font_id, font_data, glyph_data, texture_data);
  
  // Generate the string data
  Data::data_lock(font_data);
  Data::data_lock(texture_data);
  Data::data_lock(text_mesh_data);
  Data::data_lock(glyph_data);
  
  
  // Update the metrics information
  {
    uint32_t metric_texture_id = 0;
    Data::font_get_metric_texture_id(font_data, font_id, &metric_texture_id);
    
    Ogl::Texture metrics_texture;
    Data::texture_get_texture(texture_data, metric_texture_id, &metrics_texture);
  
    Ogl::texture_update_texture_1d(&metrics_texture, 0, glyph_data->size * (sizeof(float) * 4) * 4, glyph_data->field_character);
  }
  
  // Generate the text mesh here.
  // bunch of quads
  
  Graphics_api::Vertex_attribute vertdesc[3] = {
    Graphics_api::Vertex_attribute::position_3d,
    Graphics_api::Vertex_attribute::texture_coord,
    Graphics_api::Vertex_attribute::normal,    
  };
  
  Graphics_api::Vertex_format v_fmt = Graphics_api::vertex_format_create(vertdesc, 3);
  
  uint32_t glyph_info_count = strlen(str);
  Graphics_api::Quad_info *quad_info = SCRATCH_ALLOC(Graphics_api::Quad_info, glyph_info_count);
  
  const float some_scale = 0.0005;
  
  // Get the dimentions of the text
  float string_width = 0;
  float string_height = 0;
  
  for(uint32_t i = 0; i < glyph_info_count; ++i)
  {
    Text::Character *curr_glyph = &glyph_data->field_character[i];
    
    string_width += curr_glyph->advance[0];
    string_height += curr_glyph->size[1];
  }
  
  float x_cursor = 0.f - (string_width / 2.f);
  
  for(uint32_t i = 0; i < glyph_info_count; ++i)
  {
    Text::Character *curr_glyph = &glyph_data->field_character[i];
    
    quad_info[i].position[0] = 0.f;
    quad_info[i].position[1] = 0.f;
    quad_info[i].position[2] = 0.f;
    
    quad_info[i].normal[0] = 0.f;
    quad_info[i].normal[1] = 1.f;
    quad_info[i].normal[2] = 0.f;
    
    quad_info[i].scale[0] = 1.f;
    quad_info[i].scale[1] = 1.f;
    
    quad_info[i].uv[0] = 0.f;
    quad_info[i].uv[1] = 0.f;

    quad_info[i].st[0] = 1.f;
    quad_info[i].st[1] = 1.f;
    
    x_cursor += static_cast<float>(curr_glyph->advance[0]);

  }
  
  auto mesh = Graphics_api::create_quads(&v_fmt, quad_info, glyph_info_count);
  assert(Ogl::vertex_buffer_is_valid(mesh.vbo));
  
  auto text_mesh_id = Data::text_mesh_push(text_mesh_data);
  
  Data::text_mesh_set_mesh(text_mesh_data, text_mesh_id, &mesh);
  
  /*
    Add this to the text draw calls
  */
  {
    assert(text_data);
    
//    const char *str;
//    Data::text_mesh_get_text(text_mesh_data, model_id, &str);

    float advance = 0;
    float line = 0;
    int data_ptr = 0;
    
    float str_tex_data[512];
    memset(str_tex_data, 0, sizeof(str_tex_data));

    for(int i = 0; i < strlen(str); ++i)
    {
      char curr_char = str[i];
    
      // Find advance
      for(int j = 0; j < glyph_data->size; ++j)
      {
        if(str[i] == glyph_data->keys[j])
        {
          str_tex_data[data_ptr++] = j;
          str_tex_data[data_ptr++] = advance;
          str_tex_data[data_ptr++] = line;
          
          advance += glyph_data->field_character[j].advance[0];
        }
      }
    }
    
    Data::data_lock(text_data);
    
    Ogl::Texture glyph_texture;
    Ogl::Texture glyph_metrics_texture;
    stbtt_fontinfo info;
    {
      uint32_t texture_id = 0;
      uint32_t glyph_metrics_texture_id = 0;
      
      Data::font_get_font_face(font_data, font_id, &info);
      Data::font_get_glyph_texture_id(font_data, font_id, &texture_id);
      Data::font_get_metric_texture_id(font_data, font_id, &glyph_metrics_texture_id);
      
      Data::texture_get_texture(texture_data, texture_id, &glyph_texture);
      Data::texture_get_texture(texture_data, glyph_metrics_texture_id, &glyph_metrics_texture);
    }
    
    const math::transform transform = Entity_detail::get_transform(this_id, entity_data, transform_data);
    const math::mat4 world_mat = math::transform_get_world_matrix(transform);
    
    // If we don't have a draw call insert one.
    if(!Data::text_draw_call_exists(text_data, this_id))
    {
      Data::text_draw_call_push(text_data, this_id);
    }
    
    if(Data::text_draw_call_exists(text_data, this_id))
    {
      Ogl::Texture string_texture;
      Ogl::texture_create_1d(&string_texture, sizeof(float) * data_ptr, GL_RGB32F, &str_tex_data);
    
      ::Text_renderer::Draw_call dc;
      memcpy(dc.world_matrix, &world_mat, sizeof(world_mat));
      
      dc.texture       = glyph_texture;
      dc.glyph_metrics = glyph_metrics_texture;
      dc.string_info   = string_texture;
      dc.mesh          = mesh;

      Data::text_draw_call_set_draw_call(text_data, this_id, &dc);
    }

    Data::data_unlock(text_data);
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
    Data::data_lock(transform_data);
    Data::transform_set_aabb(transform_data, this_id, &return_aabb);
    Data::data_unlock(transform_data);
  }  
  
  Data::data_unlock(glyph_data);
  Data::data_unlock(text_mesh_data);
  Data::data_unlock(texture_data);
  Data::data_unlock(font_data);
}


} // ns