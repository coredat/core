#include <core/renderer/text_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/font/font.hpp>
#include <data/global_data/text_mesh_data.hpp>
#include <data/global_data/resource_data.hpp>
#include <systems/text/character.hpp>


namespace Core {


Text_renderer::Text_renderer()
: m_font_id(util::generic_id_invalid())
, m_text_id(util::generic_id_invalid())
{
  // Create a new text mesh
  auto resources = Resource_data::get_resources();
  assert(resources);
  
  auto text_mesh = resources->text_mesh_data;
  assert(text_mesh);
  
  Resource_data::data_lock(text_mesh);
  
  LOG_TODO_ONCE("Dont create a mesh every time, check to see if it exists first");
  
  m_text_id = Resource_data::text_mesh_data_push_back(text_mesh);
  
  Graphics_api::Mesh mesh;
  Resource_data::text_mesh_data_get_property_mesh(text_mesh, m_text_id, &mesh);
  
  Ogl::vertex_buffer_load(&mesh.vbo, nullptr, 0, 0, false);
  
  Resource_data::data_unlock(text_mesh);
}


void
Text_renderer::set_font(const Font &font)
{
}


Font
Text_renderer::get_font() const
{
  return Font("");
}


void
Text_renderer::set_text(const char *str)
{
  // Generate the underlying resource
  {
    auto resources = Resource_data::get_resources();
    assert(resources);
    
    auto text_mesh = resources->text_mesh_data;
    assert(text_mesh);
    
    auto font = resources->font_data;
    assert(font);
    
    auto texture = resources->texture_data;
    assert(texture);
    
    // Get the font and build the data.
    Resource_data::data_lock(font);
    Resource_data::data_lock(text_mesh);
    Resource_data::data_lock(texture);
    
    
    LOG_TODO_ONCE("Dont create a mesh every time, check to see if it exists first");
    
    
    Resource_data::text_mesh_data_set_property_text(text_mesh, m_text_id, str);

    FT_Face face;
    Resource_data::font_data_get_property_font_face(font, font->size, &face);
    
    util::generic_id texture_id;
    Resource_data::font_data_get_property_texture_id(font, font->size, &texture_id);
    
    Ogl::Texture glyph_texture;
    Resource_data::texture_data_get_property_texture(texture, texture_id, &glyph_texture);
    

    FT_Set_Pixel_Sizes(face, 0, 64);
    
    uint32_t char_index = 0;
    char curr_char = str[char_index];
    
    // Actually need to check rasterised glyphs first.
    
    static int pointer_x = 0;
    static int pointer_y = 0;
    
    while(curr_char != '\0')
    {
      // Check to see if the character exists already.
      
      
      // If it doesn't exsit already add it
      FT_Load_Char(face, curr_char, FT_LOAD_RENDER);
      curr_char = str[++char_index];
      
      if(face->glyph->bitmap.buffer)
      {
        const unsigned int width = face->glyph->bitmap.width;
        const unsigned int height = face->glyph->bitmap.rows;
      
        Text::Character glyph_details;
        glyph_details.uv[0] = math::to_float(pointer_x) / math::to_float(glyph_texture.width);
        glyph_details.uv[1] = math::to_float(pointer_y) / math::to_float(glyph_texture.height);
        
        glyph_details.st[0] = math::to_float(pointer_x + width) / math::to_float(glyph_texture.width);
        glyph_details.st[1] = math::to_float(pointer_y + height) / math::to_float(glyph_texture.height);
        
        glyph_details.advance[0] = face->glyph->advance.x;
        glyph_details.advance[1] = face->glyph->advance.y;
        
///        assert(false); // this is where I'm at
      
        Ogl::texture_update_texture_2d(&glyph_texture,
                                       pointer_x,
                                       pointer_y,
                                       width,
                                       height,
                                       face->glyph->bitmap.buffer);
        
        pointer_x += width;
      }
      else
      {
        LOG_ERROR("Failed rasterizing glyph");
      }
    }
    
//    FT_Load_Char(face, curr_char, FT_LOAD_RENDER);

    //    font_texture.update_sub_texture(0,
    //                                    0,
    //                                    face->glyph->bitmap.width,
    //                                    face->glyph->bitmap.rows,
    //                                    face->glyph->bitmap.buffer);
    
    
    Resource_data::data_unlock(texture);
    Resource_data::data_unlock(text_mesh);
    Resource_data::data_unlock(font);
  }
}


const char*
Text_renderer::get_text() const
{
  return "";
}


Text_renderer::operator Renderer() const
{
  return Renderer(Core::Renderer_type::text, m_font_id, m_text_id);
}


} // ns