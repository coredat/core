#include <transformations/font/font_resource.hpp>
#include <data/context/font_data.hpp>
#include <data/context/texture_data.hpp>
#include <utilities/logging.hpp>
#include <utilities/string_helpers.hpp>
#include <stdio.h>


namespace
{

} // anon ns



namespace Font_resource {


uint32_t
add_new_font(const char *filename,
             Data::Font_data *font_data,
             Data::Texture_data *texture_data)
{
  Data::data_lock(font_data);
  Data::data_lock(texture_data);

  // Load font data.
  stbtt_fontinfo info;
  {
    long size;
    unsigned char* fontBuffer;
    
    LOG_TODO_ONCE("Should track this allocation");
  
    FILE* fontFile = fopen(filename, "rb");
    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile); /* how long is the file ? */
    fseek(fontFile, 0, SEEK_SET); /* reset */
    
    fontBuffer = (unsigned char*)malloc(size);
    
    fread(fontBuffer, size, 1, fontFile);
    fclose(fontFile);

    if (!stbtt_InitFont(&info, fontBuffer, 0))
    {
      LOG_FATAL("Failed to load error");
      return 0;
    }
  }
  
  // Generate textures and add to data.
  uint32_t glyphs_texture_id = 0;
  uint32_t metrics_texture_id = 0;
  {
    Ogl::Texture texture_glyphs;
    Ogl::Texture texture_metrics;
  
    Ogl::texture_create_2d(&texture_glyphs, 512, 512, GL_RED, true, nullptr);
    Ogl::texture_create_1d(&texture_metrics, 512, GL_RGBA32F, nullptr);
    
    glyphs_texture_id = Data::texture_push(texture_data);
    Data::texture_set_texture(texture_data, glyphs_texture_id, &texture_glyphs);
    Data::texture_set_name(texture_data, glyphs_texture_id, "font1", strlen("font1"));
    
    metrics_texture_id = Data::texture_push(texture_data);
    Data::texture_set_texture(texture_data, metrics_texture_id, &texture_metrics);
    Data::texture_set_name(texture_data, metrics_texture_id, "font1_metrics", strlen("font1_metrics"));
  }
  
  // Generate font data.
  uint32_t font_id = 0;
  {
    char font_name[256];
    memset(font_name, 0, sizeof(font_name));
    util::filename_from_path(filename, font_name);
  
    font_id = Data::font_push(font_data);
    
    Data::font_set_name(font_data, font_id, font_name, strlen(font_name));
    
    Text::Font_bitmap font_bitmap;
    font_bitmap.bitmap_channels  = 1;
    font_bitmap.bitmap_size[0]   = 512;
    font_bitmap.bitmap_size[1]   = 512;
    font_bitmap.bitmap_offset[0] = 0;
    font_bitmap.bitmap_offset[1] = 0;
    
    Data::font_set_font_bitmap(font_data, font_id, &font_bitmap);
    Data::font_set_font_face(font_data, font_id, &info);
    Data::font_set_glyph_texture_id(font_data, font_id, &glyphs_texture_id);
    Data::font_set_metric_texture_id(font_data, font_id, &metrics_texture_id);
  }
  
  Data::data_unlock(font_data);
  Data::data_unlock(texture_data);
  
  return font_id;
}


} // ns