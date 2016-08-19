#include <core/font/font.hpp>
#include <data/global_data/resource_data.hpp>
#include <data/global_data/font_data.hpp>
#include <data/global_data/memory_data.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <3rdparty/stb/stb_truetype.h>
#include <utilities/string_helpers.hpp>


namespace
{
  //FT_Library library = nullptr;
}


namespace Core {


Font::Font(const char *filename)
{
  auto resources = Resource_data::get_resources();
  assert(resources);
  
  auto font_data = resources->font_data;
  assert(font_data);
  
  auto texture_data = resources->texture_data;
  assert(texture_data);
  
  Resource_data::data_lock(font_data);
  Resource_data::data_lock(texture_data);

  auto get_name = util::get_filename_from_path(filename);
  
  
  if(font_data_search_property_font_name(font_data, get_name.c_str(), &m_font_id))
  {
    return;
  }


  long size;
  unsigned char* fontBuffer;
  
  FILE* fontFile = fopen(filename, "rb");
  fseek(fontFile, 0, SEEK_END);
  size = ftell(fontFile); /* how long is the file ? */
  fseek(fontFile, 0, SEEK_SET); /* reset */
  
  fontBuffer = (unsigned char*)malloc(size);
  
  fread(fontBuffer, size, 1, fontFile);
  fclose(fontFile);

  stbtt_fontinfo info;
  if (!stbtt_InitFont(&info, fontBuffer, 0))
  {
    printf("failed\n");
  }
  
  
  // Generate a texture for it
  Ogl::Texture texture_glyphs;
  
  // Some empty data to zero the texture
  unsigned char *tex_data = SCRATCH_ALLOC(unsigned char, 512 * 512 * 1);
  memset(tex_data, 0, sizeof(unsigned char) * 512 * 512 * 1);
  
  Ogl::texture_create_2d(&texture_glyphs, 512, 512, GL_RED, true, tex_data);
  
  auto texture_id = Resource_data::texture_data_push_back(texture_data);
  
  Resource_data::texture_data_set_property_texture(texture_data, texture_id, texture_glyphs);

  auto font_id = font_data->size + 1;
  Resource_data::font_data_push_back(font_data, font_id);
  
  Resource_data::font_data_set_property_font_name(font_data, font_id, get_name.c_str());
  
  Text::Font_bitmap font_bitmap;
  font_bitmap.bitmap_channels  = 1;
  font_bitmap.bitmap_size[0]   = 512;
  font_bitmap.bitmap_size[1]   = 512;
  font_bitmap.bitmap_offset[0] = 0;
  font_bitmap.bitmap_offset[1] = 0;
  
  Resource_data::font_data_set_property_font_bitmap(font_data, font_id, font_bitmap);
  Resource_data::font_data_set_property_font_face(font_data, font_id, info);
  Resource_data::font_data_set_property_texture_id(font_data, font_id, texture_id);
  
  Resource_data::data_unlock(texture_data);
  Resource_data::data_unlock(font_data);
  
  m_font_id = font_id;
}


util::generic_id
Font::get_id() const
{
  return m_font_id;
}


} // ns