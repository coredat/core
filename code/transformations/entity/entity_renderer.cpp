#include <transformations/entity/entity_renderer.hpp>
#include <transformations/entity/entity_transform.hpp>
#include <transformations/text/rasterized_glyph_id.hpp>
#include <systems/text/character.hpp>
#include <core/renderer/renderer.hpp>
#include <core/renderer/material_renderer.hpp>
#include <core/renderer/text_renderer.hpp>
#include <core/resources/material.hpp>
#include <core/model/model.hpp>
#include <core/model/mesh.hpp>
#include <data/global_data/resource_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <data/world_data/world_data.hpp>
#include <data/world_data/entity_data.hpp>
#include <data/world_data/transform_data.hpp>
#include <common/error_strings.hpp>
#include <graphics_api/vertex_format.hpp>
#include <graphics_api/utils/geometry.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace Entity_detail {


uint32_t
has_renderer(const util::generic_id this_id,
             World_data::Entity_data *entity_data)
{
  World_data::data_lock(entity_data);
  
  uint32_t renderer_type(0);
  World_data::entity_data_get_property_renderer(entity_data, this_id, &renderer_type);
  
  World_data::data_unlock(entity_data);

  return renderer_type;
}


void
set_renderer(const util::generic_id this_id,
             World_data::Entity_data *entity_data,
             World_data::Transform_data *transform_data,
             World_data::Renderer_mesh_data *renderer_material,
             World_data::Renderer_text_draw_calls_data *text_data,
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
             World_data::Entity_data *entity_data,
             World_data::Renderer_mesh_data *renderer_material)
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
                      World_data::Entity_data *entity_data,
                      World_data::Transform_data *transform_data,
                      World_data::Renderer_mesh_data *mesh_data,
                      const util::generic_id material_id,
                      const util::generic_id model_id)
{
  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }
  
  // Check to see entity has a renderer.
  // If not then set it.
  {
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
      World_data::transform_data_get_property_transform(transform_data, this_id, &trans);
      
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
  
  {
    World_data::data_lock(transform_data);
    World_data::transform_data_set_property_aabb(transform_data, this_id, return_aabb);
    World_data::data_unlock(transform_data);
  }
}


void
get_renderer_material(const util::generic_id this_id,
                      World_data::Renderer_mesh_data *mesh_data,
                      util::generic_id *out_material_id,
                      util::generic_id *out_model_id)
{
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
                  World_data::Entity_data *entity_data,
                  World_data::Transform_data *transform_data,
                  World_data::Renderer_text_draw_calls_data *text_data,
                  const util::generic_id font_id,
                  const util::generic_id model_id)
{
  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }
  
  // Check to see entity has a renderer.
  // If not then set it.
  {
    LOG_TODO_ONCE("This is duplicated - unduplicate it")
  
    assert(entity_data);
    
    const uint32_t renderer_type = has_renderer(this_id, entity_data);
    
    World_data::data_lock(entity_data);
  
    if(renderer_type != 0 && (Core::Renderer_type)renderer_type != Core::Renderer_type::material)
    {
      // Currently no mechanism to change a renderer type.
      LOG_ERROR(Error_string::no_implimentation());
      return;
    }
    else
    {
      World_data::entity_data_set_property_renderer(entity_data, this_id, (uint32_t)Core::Renderer_type::text);
    }
    
    World_data::data_unlock(entity_data);
  }
  
  LOG_TODO_ONCE("Are all these resources required? - locking up a large portion of data!");
  
  auto resources = Resource_data::get_resources();
  assert(resources);
  
  auto text_mesh_data = resources->text_mesh_data;
  assert(text_mesh_data);
  
  // Set the renderer and build the mesh
  auto font_data = resources->font_data;
  assert(font_data);
  
  auto texture_data = resources->texture_data;
  assert(texture_data);
  
  auto glyph_data = resources->glyphs_data;
  assert(glyph_data);
  
  Resource_data::data_lock(font_data);
  Resource_data::data_lock(texture_data);
  Resource_data::data_lock(text_mesh_data);
  Resource_data::data_lock(glyph_data);
  
  uint32_t texture_id = 0;
  stbtt_fontinfo info;
  
  Resource_data::font_data_get_property_font_face(font_data, font_id, &info);
  Resource_data::font_data_get_property_texture_id(font_data, font_id, &texture_id);
  
  Ogl::Texture glyph_texture;
  Resource_data::texture_data_get_property_texture(texture_data, texture_id, &glyph_texture);
  
  const int bitmap_width  = glyph_texture.width; /* bitmap width */
  const int bitmap_height = glyph_texture.height; /* bitmap height */
  int l_h = 64; /* line height */

  /* create a bitmap for the phrase */
  unsigned char* bitmap = (unsigned char*)malloc(bitmap_width * bitmap_height);
  memset(bitmap, 0, sizeof(unsigned char) * (bitmap_width * bitmap_height));
  
  /* calculate font scaling */
  const float scale = stbtt_ScaleForPixelHeight(&info, l_h);

  int ascent, descent, lineGap;
  stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
  
//  Text::Character char_props;
//  char_props.advance[0] = ascent;
//  char_props.advance[1] = descent;
  
  ascent *= scale;
  descent *= scale;
  
  Text::Font_bitmap font_bitmap;
  Resource_data::font_data_get_property_font_bitmap(font_data, font_id, &font_bitmap);
  font_bitmap.line_height = ascent + math::abs(descent);
  font_bitmap.bitmap_size[0] = glyph_texture.width;
  font_bitmap.bitmap_size[1] = glyph_texture.height;
  
  Text::Character *glyph_info = nullptr;
  uint32_t glyph_info_count = 0;
  
  // Generate missing glyphs
  {
    const char *str;
    Resource_data::text_mesh_data_get_property_text(text_mesh_data, model_id, &str);
    
    glyph_info = SCRATCH_ALIGNED_ALLOC(Text::Character, strlen(str) * sizeof(Text::Character));
  
    for(int i = 0; i < strlen(str); ++i)
    {
      const int codepoint = str[i];
      
      // If code point exists skip
      const uint32_t glyph_id = Text::create_glyph_id(font_id, codepoint);
      
      if(Resource_data::rasterized_glyphs_data_exists(glyph_data, glyph_id))
      {
        Resource_data::rasterized_glyphs_data_get_property_character(glyph_data,
                                                                     glyph_id,
                                                                     &glyph_info[glyph_info_count]);
        ++glyph_info_count;
        
        continue;
      }
      
      int glyph_width, glyph_height;
      int x_offset, y_offset;
      
      unsigned char * glyph_bitmap = stbtt_GetCodepointBitmap(&info,
                                                              0,
                                                              scale,
                                                              codepoint,
                                                              &glyph_width,
                                                              &glyph_height,
                                                              &x_offset,
                                                              &y_offset);

  //    const math::vec2 offset = math::vec2_init(bitmap_width * 0.5 + x_offset, -(bitmap_height * 0.5f + y_offset));

      int advance, left_side_bearing;
      stbtt_GetCodepointHMetrics(&info, codepoint, &advance, &left_side_bearing);
      
      const int bitmap_advance = glyph_width; //math::max((int32_t)advance_ft, glyph_width);
      const int width_needed = bitmap_advance + glyph_width;
      
      if(font_bitmap.bitmap_offset[0] + width_needed > bitmap_width)
      {
        if(font_bitmap.bitmap_offset[1] + font_bitmap.line_height > font_bitmap.bitmap_size[1])
        {
          LOG_WARNING("Font map is full.");
          break;
        }
        
        font_bitmap.bitmap_offset[0] = 0;
        font_bitmap.bitmap_offset[1] += font_bitmap.line_height;
      }
      
      Ogl::texture_update_texture_2d(&glyph_texture,
                                     font_bitmap.bitmap_offset[0],
                                     font_bitmap.bitmap_offset[1],
                                     glyph_width,
                                     glyph_height,
                                     glyph_bitmap);
      
       int kern;
      kern = stbtt_GetCodepointKernAdvance(&info, codepoint, str[i + 1]);
      
      stbtt_FreeBitmap(glyph_bitmap, nullptr);
      
      // Set the glyph properties.
      Text::Character char_info;

      char_info.size[0] = glyph_width;
      char_info.size[1] = glyph_height;
      
      char_info.offset[0] = x_offset;
      char_info.offset[1] = y_offset;

      char_info.advance[0] = (advance * scale);
      char_info.uv[0] = math::to_float(font_bitmap.bitmap_offset[0]) / math::to_float(font_bitmap.bitmap_size[0]);
      char_info.uv[1] = math::to_float(font_bitmap.bitmap_offset[1]) / math::to_float(font_bitmap.bitmap_size[1]);
      
      char_info.st[0] = char_info.uv[0] + (math::to_float(glyph_width) / math::to_float(font_bitmap.bitmap_size[0]));
      char_info.st[1] = char_info.uv[1] + (math::to_float(glyph_height) / math::to_float(font_bitmap.bitmap_size[1]));
      
      // We can now add the advance
      font_bitmap.bitmap_offset[0] += bitmap_advance + 2;
      
      // Add glyph info
      Resource_data::rasterized_glyphs_data_push_back(glyph_data, glyph_id);
      Resource_data::rasterized_glyphs_data_set_property_character(glyph_data, glyph_id, char_info);
      
      // Also add it to the glyph info array.
      glyph_info[glyph_info_count++] = char_info;
    }
  } // gen missing glyphs
  
  // Generate the text mesh here.
  // bunch of quads
  
  Graphics_api::Vertex_attribute vertdesc[3] = {
    Graphics_api::Vertex_attribute::position_3d,
    Graphics_api::Vertex_attribute::normal,
    Graphics_api::Vertex_attribute::texture_coord,
  };
  
  Graphics_api::Vertex_format v_fmt = Graphics_api::vertex_format_create(vertdesc, 3);
  
  Graphics_api::Quad_info *quad_info = SCRATCH_ALLOC(Graphics_api::Quad_info, glyph_info_count);
  
  const float some_scale = 1.f;
  
  // Get the dimentions of the text
  float string_width = 0;
  float string_height = 0;
  
  for(uint32_t i = 0; i < glyph_info_count; ++i)
  {
    Text::Character *curr_glyph = &glyph_info[i];
    
    string_width += curr_glyph->advance[0];
    string_height += curr_glyph->size[1];
  }
  
  float x_cursor = 0.f - (string_width / 2.f);
  
  for(uint32_t i = 0; i < glyph_info_count; ++i)
  {
    Text::Character *curr_glyph = &glyph_info[i];
    
    quad_info[i].position[0] = (x_cursor + curr_glyph->offset[0]) * some_scale;
    quad_info[i].position[1] = -(curr_glyph->size[1] + curr_glyph->offset[1]) * some_scale;
    quad_info[i].position[2] = 0.f;
    
    quad_info[i].normal[0] = 0.f;
    quad_info[i].normal[1] = 1.f;
    quad_info[i].normal[2] = 0.f;
    
    quad_info[i].scale[0] = curr_glyph->size[0] * some_scale;
    quad_info[i].scale[1] = curr_glyph->size[1] * some_scale;
    
    quad_info[i].uv[0] = curr_glyph->uv[0];
    quad_info[i].uv[1] = curr_glyph->uv[1];

    quad_info[i].st[0] = curr_glyph->st[0];
    quad_info[i].st[1] = curr_glyph->st[1];
    
    x_cursor += static_cast<float>(curr_glyph->advance[0]);
  }
  
  auto mesh = Graphics_api::create_quads(&v_fmt, quad_info, glyph_info_count);
  assert(Ogl::vertex_buffer_is_valid(mesh.vbo));
  
  auto text_mesh_id = Resource_data::text_mesh_data_push_back(text_mesh_data);
  
  Resource_data::text_mesh_data_set_property_mesh(text_mesh_data, text_mesh_id, mesh);
  
  /*
    Add this to the text draw calls
  */
  {
    assert(text_data);
    
    World_data::data_lock(text_data);
    
    const math::transform transform = Entity_detail::get_transform(this_id, entity_data, transform_data);
    const math::mat4 world_mat = math::transform_get_world_matrix(transform);
    
    // If we don't have a draw call insert one.
    if(!World_data::renderer_text_draw_calls_data_exists(text_data, this_id))
    {
      World_data::renderer_text_draw_calls_data_push_back(text_data, this_id);
    }
    
    if(World_data::renderer_text_draw_calls_data_exists(text_data, this_id))
    {
      ::Text_renderer::Draw_call dc;
      dc.texture = glyph_texture;

      memcpy(dc.world_matrix, &world_mat, sizeof(world_mat));

      dc.mesh = mesh;

      World_data::renderer_text_draw_calls_data_set_property_draw_call(text_data, this_id, &dc);
    }

    World_data::data_unlock(text_data);
  }
  
  Resource_data::data_unlock(glyph_data);
  Resource_data::data_unlock(text_mesh_data);
  Resource_data::data_unlock(texture_data);
  Resource_data::data_unlock(font_data);
}


} // ns