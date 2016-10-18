#include <core/font/font.hpp>
#include <data/context_data.hpp>
#include <data/memory/memory_data.hpp>
#include <data/context/font_data.hpp>
#include <data/context/texture_data.hpp>
#include <utilities/string_helpers.hpp>
#include <3rdparty/stb/stb_truetype.h>
#include <stdio.h>
#include <stdlib.h>



namespace
{
  //FT_Library library = nullptr;
}


namespace Core {


Font::Font(const char *filename)
{
  auto resources = Data::get_context_data();
  assert(resources);
  
  auto font_data = resources->font_data;
  assert(font_data);
  
  auto texture_data = resources->texture_data;
  assert(texture_data);
  
  Data::data_lock(font_data);
  Data::data_lock(texture_data);

  auto get_name = util::get_filename_from_path(filename);
  
  auto
  search_name = [](const auto *data, const char *value, util::generic_id *out_key) -> bool
  {
    LOG_TODO_ONCE("This is a hack solve it.");
    bool found = false;

    for(size_t i = 0; i < data->size; ++i)
    {
      if(!strcmp(value, &data->field_name[i * 32]))
      {
        found = true;

        if(out_key)
        {
          *out_key = data->keys[i];
        }

        break;
      }
    }

    return found;
  };
  
  if(search_name(font_data, get_name.c_str(), &m_font_id))
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
  
  const uint32_t texture_id = Data::texture_push(texture_data);
  
  Data::texture_set_texture(texture_data, texture_id, &texture_glyphs);

  const uint32_t font_id = Data::font_push(font_data);
  
  Data::font_set_name(font_data, font_id, get_name.c_str(), strlen(get_name.c_str()));
  
  Text::Font_bitmap font_bitmap;
  font_bitmap.bitmap_channels  = 1;
  font_bitmap.bitmap_size[0]   = 512;
  font_bitmap.bitmap_size[1]   = 512;
  font_bitmap.bitmap_offset[0] = 0;
  font_bitmap.bitmap_offset[1] = 0;
  
  Data::font_set_font_bitmap(font_data, font_id, &font_bitmap);
  Data::font_set_font_face(font_data, font_id, &info);
  Data::font_set_texture_id(font_data, font_id, &texture_id);
  
  Data::data_unlock(texture_data);
  Data::data_unlock(font_data);
  
  m_font_id = font_id;
}


util::generic_id
Font::get_id() const
{
  return m_font_id;
}


} // ns