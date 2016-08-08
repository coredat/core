#include <core/font/font.hpp>
#include <data/global_data/resource_data.hpp>
#include <data/global_data/font_data.hpp>
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
  
  auto resources = Resource_data::get_resources();
  assert(resources);
  
  auto font_data = resources->font_data;
  assert(font_data);
  
  auto texture_data = resources->texture_data;
  assert(texture_data);
  
  Resource_data::data_lock(font_data);
  Resource_data::data_lock(texture_data);
  
  
  // Generate a texture for it
  Ogl::Texture texture_glyphs;
  Ogl::texture_create_2d(&texture_glyphs, 512, 512, GL_RED, true, nullptr);
  
  auto texture_id = Resource_data::texture_data_push_back(texture_data);
  
  Resource_data::texture_data_set_property_texture(texture_data, texture_id, texture_glyphs);

  auto font_id = font_data->size + 1;
  Resource_data::font_data_push_back(font_data, font_id);
  
  auto get_name = util::get_filename_from_path(filename);
  
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
  
  id = font_id;
  
//  int b_w = 512; /* bitmap width */
//  int b_h = 512; /* bitmap height */
//  int l_h = 64; /* line height */
//
//  /* create a bitmap for the phrase */
//  unsigned char* bitmap = (unsigned char*)malloc(b_w * b_h);
//  
//  /* calculate font scaling */
//  float scale = stbtt_ScaleForPixelHeight(&info, l_h);
//
//  int ascent, descent, lineGap;
//  stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
//  
//  ascent *= scale;
//  descent *= scale;
//
//  const char * str = "foobar!";
//
//  int x = 0;
//  int i;
//  for (i = 0; i < strlen(str); ++i)
//  {
//    /* get bounding box for character (may be offset to account for chars that dip above or below the line */
//    int c_x1, c_y1, c_x2, c_y2;
//    stbtt_GetCodepointBitmapBox(&info, str[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
//    
//    /* compute y (different characters have different heights */
//    int y = ascent + c_y1;
//    
//    /* render character (stride and offset is important here) */
//    int byteOffset = x + (y  * b_w);
//    stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, str[i]);
//    
//    /* how wide is this character */
//    int ax;
//    stbtt_GetCodepointHMetrics(&info, str[i], &ax, 0);
//    x += ax * scale;
//    
//    /* add kerning */
//    int kern;
//    kern = stbtt_GetCodepointKernAdvance(&info, str[i], str[i + 1]);
//    x += kern * scale;
//  }

  /* save out a 1 channel image */
  //stbi_write_png("out.png", b_w, b_h, 1, bitmap, b_w);
  
  //free(fontBuffer);
  //free(bitmap);

//  if(!library)
//  {
//    auto error = FT_Init_FreeType( &library );
//    if ( error )
//    {
//      LOG_ERROR("Failed init freetype lib");
//    }
//  }

//  FT_Face face;      /* handle to face object */
//  auto error = FT_New_Face(library,
//                            filename,
//                            0,
//                            &face);
//  if(error)
//  {
//    LOG_ERROR("Font err")
//  }
//  
//  auto resources = Resource_data::get_resources();
//  auto font = resources->font_data;
//  auto texture = resources->texture_data;
//
//  Resource_data::data_lock(font);
//  Resource_data::data_lock(texture);
//  
//  Resource_data::font_data_push_back(font, font->size + 1);
//  Resource_data::font_data_set_property_font_face(font, font->size, face);
//  
//  auto texture_id = Resource_data::texture_data_push_back(texture);
//  
//  Ogl::Texture texture_glyphs;
//  Ogl::texture_create_2d(&texture_glyphs, 512, 512, GL_RED, true, nullptr);
//  
//  Resource_data::texture_data_set_property_texture(texture, texture_id, texture_glyphs);
//  Resource_data::font_data_set_property_texture_id(font, font->size, texture_id);
//  
//  Resource_data::data_unlock(texture);
//  Resource_data::data_unlock(font);
}


util::generic_id
Font::get_id() const
{
  return id;
}


} // ns