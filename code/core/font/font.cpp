#include <core/font/font.hpp>
#include <data/global_data/resource_data.hpp>
#include <data/global_data/font_data.hpp>


namespace
{
  FT_Library library = nullptr;
}


namespace Core {


Font::Font(const char *filename)
{
  if(!library)
  {
    auto error = FT_Init_FreeType( &library );
    if ( error )
    {
      LOG_ERROR("Failed init freetype lib");
    }
  }

  FT_Face face;      /* handle to face object */
  auto error = FT_New_Face(library,
                            filename,
                            0,
                            &face);
  if(error)
  {
    LOG_ERROR("Font err")
  }
  
  auto resources = Resource_data::get_resources();
  auto font = resources->font_data;
  auto texture = resources->texture_data;

  Resource_data::data_lock(font);
  Resource_data::data_lock(texture);
  
  Resource_data::font_data_push_back(font, font->size + 1);
  Resource_data::font_data_set_property_font_face(font, font->size, face);
  
  auto texture_id = Resource_data::texture_data_push_back(texture);
  
  Ogl::Texture texture_glyphs;
  Ogl::texture_create_2d(&texture_glyphs, 512, 512, GL_RED, true, nullptr);
  
  Resource_data::texture_data_set_property_texture(texture, texture_id, texture_glyphs);
  Resource_data::font_data_set_property_texture_id(font, font->size, texture_id);
  
  Resource_data::data_unlock(texture);
  Resource_data::data_unlock(font);
}


} // ns