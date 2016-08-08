#include <core/renderer/text_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/font/font.hpp>
#include <data/global_data/text_mesh_data.hpp>
#include <data/global_data/resource_data.hpp>
#include <systems/text/character.hpp>

#include <graphics_api/utils/geometry.hpp>
#include <graphics_api/mesh.hpp>
#include <graphics_api/vertex_format.hpp>


namespace Core {


Text_renderer::Text_renderer()
: m_font_id(util::generic_id_invalid())
, m_text_id(util::generic_id_invalid())
{
//  // Create a new text mesh
//  auto resources = Resource_data::get_resources();
//  assert(resources);
//  
//  auto text_mesh = resources->text_mesh_data;
//  assert(text_mesh);
//  
//  Resource_data::data_lock(text_mesh);
//  
//  LOG_TODO_ONCE("Dont create a mesh every time, check to see if it exists first");
//  
//  m_text_id = Resource_data::text_mesh_data_push_back(text_mesh);
//  
//  Graphics_api::Mesh mesh;
//  Resource_data::text_mesh_data_get_property_mesh(text_mesh, m_text_id, &mesh);
//  
//  Ogl::vertex_buffer_load(&mesh.vbo, nullptr, 0, 0, false);
//  
//  Resource_data::data_unlock(text_mesh);
}


Text_renderer::Text_renderer(const util::generic_id font_id,
                             const util::generic_id text_id)
: m_font_id(font_id)
, m_text_id(text_id)
{
  LOG_TODO("Check these ids are valid.");
}


void
Text_renderer::set_font(const Font &font)
{
  m_font_id = font.get_id();
}


Font
Text_renderer::get_font() const
{
  return Font("");
}


void
Text_renderer::set_text(const char *str)
{
  auto resources = Resource_data::get_resources();
  assert(resources);
  
  auto font_data = resources->font_data;
  assert(font_data);
  
  auto texture_data = resources->texture_data;
  assert(texture_data);
  
  auto text_mesh_data = resources->text_mesh_data;
  assert(text_mesh_data);
  
  Resource_data::data_lock(font_data);
  Resource_data::data_lock(texture_data);
  Resource_data::data_lock(text_mesh_data);
  
  uint32_t texture_id = 0;
  stbtt_fontinfo info;
  
  Resource_data::font_data_get_property_font_face(font_data, m_font_id, &info);
  Resource_data::font_data_get_property_texture_id(font_data, m_font_id, &texture_id);
  
  m_text_id = Resource_data::text_mesh_data_push_back(text_mesh_data);
  
  Ogl::Texture glyph_texture;
  Resource_data::texture_data_get_property_texture(texture_data, texture_id, &glyph_texture);
  
  const int bitmap_width  = glyph_texture.width; /* bitmap width */
  const int bitmap_height = glyph_texture.height; /* bitmap height */
  int l_h = 64; /* line height */

  /* create a bitmap for the phrase */
  unsigned char* bitmap = (unsigned char*)malloc(bitmap_width * bitmap_height);
  memset(bitmap, 0, sizeof(unsigned char) * (bitmap_width * bitmap_height));
  
  /* calculate font scaling */
  float scale = stbtt_ScaleForPixelHeight(&info, l_h);

  int ascent, descent, lineGap;
  stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
  
//  Text::Character char_props;
//  char_props.advance[0] = ascent;
//  char_props.advance[1] = descent;
  
  ascent *= scale;
  descent *= scale;

  int x = 0;
  int i;
  
  Text::Character char_props[64];
  
  for (i = 0; i < strlen(str); ++i) // utf8 support?
  {
    const float u = math::to_float(x) / 512.f;
    const float v = math::to_float(0) / 512.f;
  
  
    const int codepoint = str[i];
    const int next_codepoint = str[i + 1]; // Err! out of bounds?
    
    /* get bounding box for character (may be offset to account for chars that dip above or below the line */
    int c_x1, c_y1, c_x2, c_y2;
    stbtt_GetCodepointBitmapBox(&info, codepoint, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
    
    /* compute y (different characters have different heights */
    int y = ascent + c_y1;
    
    /* render character (stride and offset is important here) */
    int byteOffset = x + (y  * bitmap_width);
    stbtt_MakeCodepointBitmap(&info, &bitmap[byteOffset], c_x2 - c_x1, c_y2 - c_y1, bitmap_width, scale, scale, codepoint);
    
    /* how wide is this character */
    int ax;
    stbtt_GetCodepointHMetrics(&info, codepoint, &ax, 0);
    x += ax * scale;
    
    int ay;
    
    /* add kerning */
    int kern;
    kern = stbtt_GetCodepointKernAdvance(&info, codepoint, next_codepoint);
    x += kern * scale;
    
    const int glyph_width = c_x2 - c_x1;
    const int glyph_height = c_y2 - c_y1;
    
    
    const float s = math::to_float(glyph_width) / 512.f;
    const float t = math::to_float(glyph_height) / 512.f;
    
    {
      char_props[i].advance[0] = ascent;
      char_props[i].advance[1] = descent;
      
      char_props[i].uv[0] = u;
      char_props[i].uv[1] = v;
      
      char_props[i].st[0] = s;
      char_props[i].st[1] = t;
    }
  }
  
  Ogl::texture_update_texture_2d(&glyph_texture, 0, 0, 512, 512, bitmap);
  
  // Generate the text mesh here.
  // bunch of quads
  
  Graphics_api::Vertex_attribute vertdesc[3] = {
    Graphics_api::Vertex_attribute::position_3d,
    Graphics_api::Vertex_attribute::normal,
    Graphics_api::Vertex_attribute::texture_coord,
  };
  
  Graphics_api::Vertex_format v_fmt = Graphics_api::vertex_format_create(vertdesc, 3);
  
  Graphics_api::Quad_info q;
  q.position[0] = 0.f;
  q.position[1] = 0.f;
  q.position[2] = 0.f;
  
  q.normal[0] = 0.f;
  q.normal[1] = 1.f;
  q.normal[2] = 0.f;
  
  q.uv[0] = char_props[0].uv[0];
  q.uv[1] = char_props[0].uv[1];

  q.st[0] = char_props[0].st[0];
  q.st[1] = char_props[0].st[1];
  
  auto mesh = Graphics_api::create_quads(&v_fmt, &q, 1);
  assert(Ogl::vertex_buffer_is_valid(mesh.vbo));
  
  auto text_mesh_id = Resource_data::text_mesh_data_push_back(text_mesh_data);
  
  Resource_data::text_mesh_data_set_property_text(text_mesh_data, text_mesh_id, str);
  Resource_data::text_mesh_data_set_property_mesh(text_mesh_data, text_mesh_id, mesh);
  
  Resource_data::data_unlock(text_mesh_data);
  Resource_data::data_lock(texture_data);
  Resource_data::data_lock(font_data);

//  return;
//  // Generate the underlying resource
//  {
//    auto resources = Resource_data::get_resources();
//    assert(resources);
//    
//    auto text_mesh = resources->text_mesh_data;
//    assert(text_mesh);
//    
//    auto font = resources->font_data;
//    assert(font);
//    
//    auto texture = resources->texture_data;
//    assert(texture);
//    
//    auto glyphs = resources->glyphs_data;
//    assert(glyphs);
//    
//    // Get the font and build the data.
//    Resource_data::data_lock(font);
//    Resource_data::data_lock(text_mesh);
//    Resource_data::data_lock(texture);
//    Resource_data::data_lock(glyphs);
//    
//    LOG_TODO_ONCE("Dont create a mesh every time, check to see if it exists first");
//    
//    
//    Resource_data::text_mesh_data_set_property_text(text_mesh, m_text_id, str);
//
//    FT_Face face;
//    Resource_data::font_data_get_property_font_face(font, font->size, &face);
//    
//    if(face == nullptr)
//    {
//      return;
//    }
//    
//    util::generic_id texture_id;
//    Resource_data::font_data_get_property_texture_id(font, font->size, &texture_id);
//    
//    Ogl::Texture glyph_texture;
//    Resource_data::texture_data_get_property_texture(texture, texture_id, &glyph_texture);
//    
//
//    FT_Set_Pixel_Sizes(face, 0, 64);
//    
//    uint32_t char_index = 0;
//    char curr_char = str[char_index];
//    
//    // Actually need to check rasterised glyphs first.
//    
//    static int pointer_x = 0;
//    static int pointer_y = 0;
//    
//    while(curr_char != '\0')
//    {
//      // Check to see if the character exists already.
//      
//      
//      // If it doesn't exsit already add it
//      FT_Load_Char(face, curr_char, FT_LOAD_RENDER);
//      
//      if(face->glyph->bitmap.buffer)
//      {
//        const unsigned int width = face->glyph->bitmap.width;
//        const unsigned int height = face->glyph->bitmap.rows;
//      
//        Text::Character glyph_details;
//        glyph_details.uv[0] = math::to_float(pointer_x) / math::to_float(glyph_texture.width);
//        glyph_details.uv[1] = math::to_float(pointer_y) / math::to_float(glyph_texture.height);
//        
//        glyph_details.st[0] = math::to_float(pointer_x + width) / math::to_float(glyph_texture.width);
//        glyph_details.st[1] = math::to_float(pointer_y + height) / math::to_float(glyph_texture.height);
//        
//        glyph_details.advance[0] = face->glyph->advance.x;
//        glyph_details.advance[1] = face->glyph->advance.y;
//        
////        assert(false); // this is where I'm at
//      
//        Ogl::texture_update_texture_2d(&glyph_texture,
//                                       pointer_x,
//                                       pointer_y,
//                                       width,
//                                       height,
//                                       face->glyph->bitmap.buffer);
//        
//        Resource_data::rasterized_glyphs_data_push_back(glyphs, (uint32_t)curr_char);
//        Resource_data::rasterized_glyphs_data_set_property_character(glyphs, (uint32_t)curr_char, glyph_details);
//        
//        pointer_x += width;
//      }
//      else
//      {
//        LOG_ERROR("Failed rasterizing glyph");
//      }
//      
//      curr_char = str[++char_index];
//    }
//    
//    
//    // Build mesh
//    {
//      LOG_TODO_ONCE("Font might not be set here!")
//      
//      uint32_t char_index = 0;
//      char curr_char = str[char_index];
//    
//      std::vector<float> data;
//      
//      float advance = 0;
//    
//      while(curr_char != '\0')
//      {
//        // Find glyph and build a vertex
//        Text::Character glyph_details;
//        Resource_data::rasterized_glyphs_data_get_property_character(glyphs, (util::generic_id)curr_char, &glyph_details);
//        
//        curr_char = str[++char_index];
//      }
//    }
//    
//    Resource_data::data_unlock(glyphs);
//    Resource_data::data_unlock(texture);
//    Resource_data::data_unlock(text_mesh);
//    Resource_data::data_unlock(font);
//  }
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


util::generic_id
Text_renderer::get_font_id() const
{
  return m_font_id;
}


util::generic_id
Text_renderer::get_text_id() const
{
  return m_text_id;
}


} // ns