#include <transformations/font/font_resource.hpp>
#include <transformations/text/rasterized_glyph_id.hpp>
#include <transformations/entity/entity_transform.hpp>
#include <data/context/font_data.hpp>
#include <data/context/font_glyph_data.hpp>
#include <data/context/texture_data.hpp>
#include <data/memory/memory_data.hpp>
#include <data/world/entity_data.hpp>
#include <data/world/text_draw_call_data.hpp>
#include <data/world/transform_data.hpp>
#include <systems/text/character.hpp>
#include <math/math.hpp>
#include <utilities/logging.hpp>
#include <utilities/string_helpers.hpp>
#include <op/op.hpp>
#include <stdio.h>


namespace
{

} // anon ns



namespace Font_resource {


uint32_t
add_new_font(const char *filename,
             Data::Font_data *font_data,
             Data::Texture_data *texture_data,
             opContext *ctx,
             opBuffer *buf)
{
  Data::data_lock(font_data);
  Data::data_lock(texture_data);

  // Load font data.
  stbtt_fontinfo info;
  Text::Font_bitmap font_bitmap;
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
//    Ogl::Texture texture_glyphs;
//    Ogl::Texture texture_metrics;
//  
//    Ogl::texture_create_2d(&texture_glyphs, 512, 512, GL_RED, true, nullptr);
//    Ogl::texture_create_1d(&texture_metrics, 512, GL_RGBA32F, nullptr);

    opTextureDesc glyph_desc;
    memset(&glyph_desc, 0, sizeof(glyph_desc));
    glyph_desc.width = 512;
    glyph_desc.height = 512;
    glyph_desc.format = opPixelFormat_R8;
    glyph_desc.dimention = opDimention_TWO;
    
    glyphs_texture_id = opBufferTextureCreate(ctx, buf, nullptr, &glyph_desc);
    
    opTextureDesc metrics_desc;
    memset(&metrics_desc, 0, sizeof(metrics_desc));
    metrics_desc.width = 512;
    metrics_desc.format = opPixelFormat_RGBA32F;
    metrics_desc.dimention = opDimention_ONE;
    
    metrics_texture_id = opBufferTextureCreate(ctx, buf, nullptr, &metrics_desc);
    opBufferExec(ctx, buf);
    
//    glyphs_texture_id = Data::texture_push(texture_data);
//    Data::texture_set_texture(texture_data, glyphs_texture_id, &texture_glyphs);
//    Data::texture_set_name(texture_data, glyphs_texture_id, "font1", strlen("font1"));
    
//    metrics_texture_id = Data::texture_push(texture_data);
//    Data::texture_set_texture(texture_data, metrics_texture_id, &texture_metrics);
//    Data::texture_set_name(texture_data, metrics_texture_id, "font1_metrics", strlen("font1_metrics"));
  }
  
  // Font info
  {
    // Get some info out of the font.
    const int height = 64;
    const float scale = stbtt_ScaleForPixelHeight(&info, height);
    
    int ascent, descent, line_gap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &line_gap);
    
    LOG_TODO_ONCE("Font Bitmap values are hardcoded - dont");
    
    font_bitmap.bitmap_channels  = 1;
    font_bitmap.bitmap_size[0]   = 512;
    font_bitmap.bitmap_size[1]   = 512;
    font_bitmap.bitmap_offset[0] = 0;
    font_bitmap.bitmap_offset[1] = 0;
    font_bitmap.line_height      = math::to_float(line_gap) * scale;
    font_bitmap.scale            = scale;
    font_bitmap.decsent          = math::to_float(descent) * scale;
    font_bitmap.ascent           = math::to_float(ascent) * scale;
  }
  
  // Generate font data.
  uint32_t font_id = 0;
  {
    char font_name[256];
    memset(font_name, 0, sizeof(font_name));
    util::filename_from_path(filename, font_name);
  
    font_id = Data::font_push(font_data);
    
    Data::font_set_name(font_data, font_id, font_name, strlen(font_name));
    
    Data::font_set_font_bitmap(font_data, font_id, &font_bitmap);
    Data::font_set_font_face(font_data, font_id, &info);
    Data::font_set_glyph_texture_id(font_data, font_id, &glyphs_texture_id);
    Data::font_set_metric_texture_id(font_data, font_id, &metrics_texture_id);
  }
  
  Data::data_unlock(font_data);
  Data::data_unlock(texture_data);
  
  return font_id;
}


void
add_glyphs(const char *glyph_arr,
           const size_t glyph_arr_count,
           const uint32_t font_id,
           Data::Font_data *font_data,
           Data::Font_glyph_data *glyph_data,
           Data::Texture_data *texture_data,
           opContext *ctx,
           opBuffer *buf)
{
  Data::data_lock(font_data);
  Data::data_lock(texture_data);
  Data::data_lock(glyph_data);
  
  // Get various font infos
  opID glyph_texture = 0;
  opID glyph_metrics_texture = 0;
  stbtt_fontinfo info;
  Text::Font_bitmap font_bitmap;
  {
    Data::font_get_font_face(font_data, font_id, &info);
    Data::font_get_glyph_texture_id(font_data, font_id, &glyph_texture);
    Data::font_get_metric_texture_id(font_data, font_id, &glyph_metrics_texture);
    Data::font_get_font_bitmap(font_data, font_id, &font_bitmap);
    
//    Data::texture_get_texture(texture_data, texture_id, &glyph_texture);
//    Data::texture_get_texture(texture_data, glyph_metrics_texture_id, &glyph_metrics_texture);
  }
  
  for(int i = 0; i < strlen(glyph_arr); ++i)
  {
    const int codepoint = glyph_arr[i];
    
    // If code point exists skip
    const uint32_t glyph_id = Text::create_glyph_id(font_id, codepoint);
    
    if(Data::font_glyph_exists(glyph_data, glyph_id))
    {
      continue;
    }
    
    if(codepoint == '\n')
    {
      continue;
    }
    
    // Create a new glyph
    
    int glyph_width, glyph_height;
    int x_offset, y_offset;
    
    unsigned char * glyph_bitmap = stbtt_GetCodepointBitmap(&info,
                                                            0,
                                                            font_bitmap.scale,
                                                            codepoint,
                                                            &glyph_width,
                                                            &glyph_height,
                                                            &x_offset,
                                                            &y_offset);


    int advance, left_side_bearing;
    stbtt_GetCodepointHMetrics(&info, codepoint, &advance, &left_side_bearing);
    
    const int bitmap_advance = glyph_width;
    const int width_needed = bitmap_advance + glyph_width;
    
    if(font_bitmap.bitmap_offset[0] + width_needed > font_bitmap.bitmap_size[0])
    {
      if(font_bitmap.bitmap_offset[1] + font_bitmap.line_height > font_bitmap.bitmap_size[1])
      {
        LOG_WARNING("Font map is full.");
        break;
      }
      
      font_bitmap.bitmap_offset[0] = 0;
      font_bitmap.bitmap_offset[1] += font_bitmap.line_height;
    }

    opBufferExec(ctx, buf);
    opBufferTextureUpdate(ctx, buf, glyph_texture, font_bitmap.bitmap_offset[0], font_bitmap.bitmap_offset[1], glyph_data);
    opBufferExec(ctx, buf);
    
//    Ogl::texture_update_texture_2d(&glyph_texture,
//                                   font_bitmap.bitmap_offset[0],
//                                   font_bitmap.bitmap_offset[1],
//                                   glyph_width,
//                                   glyph_height,
//                                   glyph_bitmap);
    
    int kern;
    kern = stbtt_GetCodepointKernAdvance(&info, codepoint, glyph_arr[i + 1]);
    
    stbtt_FreeBitmap(glyph_bitmap, nullptr);
    
    // Set the glyph properties.
    Text::Character char_info;
    
    char_info.size[0] = glyph_width;
    char_info.size[1] = glyph_height;
    
    char_info.offset[0] = x_offset;
    char_info.offset[1] = y_offset;

    char_info.advance[0] = (math::to_float(advance) * font_bitmap.scale) / math::to_float(font_bitmap.bitmap_size[0]);
    char_info.advance[1] = (font_bitmap.line_height + font_bitmap.decsent) * font_bitmap.scale;
    
    char_info.uv[0] = math::to_float(font_bitmap.bitmap_offset[0]) / math::to_float(font_bitmap.bitmap_size[0]);
    char_info.uv[1] = math::to_float(font_bitmap.bitmap_offset[1]) / math::to_float(font_bitmap.bitmap_size[1]);
    
    char_info.st[0] = char_info.uv[0] + (math::to_float(glyph_width) / math::to_float(font_bitmap.bitmap_size[0]));
    char_info.st[1] = char_info.uv[1] + (math::to_float(glyph_height) / math::to_float(font_bitmap.bitmap_size[1]));
    
    // We can now add the advance
    font_bitmap.bitmap_offset[0] += bitmap_advance + 2;
    
    // Add glyph info
    Data::font_glyph_push(glyph_data, glyph_id);
    Data::font_glyph_set_character(glyph_data, glyph_id, &char_info);

    opBufferTextureUpdate(ctx, buf, glyph_metrics_texture, 0, glyph_data->size * 5, glyph_data->field_character);
    opBufferExec(ctx, buf);
    
//    Ogl::texture_update_texture_1d(&glyph_metrics_texture,
//                                   0,
//                                   glyph_data->size * 5,
//                                   glyph_data->field_character);
  }
  
  
  // Update the metrics information
  {
    Data::font_set_font_bitmap(font_data, font_id, &font_bitmap);
    
    uint32_t metric_texture_id = 0;
    Data::font_get_metric_texture_id(font_data, font_id, &metric_texture_id);
    
    Ogl::Texture metrics_texture;
    Data::texture_get_texture(texture_data, metric_texture_id, &metrics_texture);
  
    Ogl::texture_update_texture_1d(&metrics_texture, 0, glyph_data->size * (sizeof(float) * 4) * 4, glyph_data->field_character);
  }
  
  
  Data::data_unlock(glyph_data);
  Data::data_unlock(texture_data);
  Data::data_unlock(font_data);
}


void
create_string_data(const util::generic_id this_id,
                   const char *glyph_arr,
                   const size_t glyph_arr_count,
                   const uint32_t font_id,
                   Data::Font_data *font_data,
                   Data::Font_glyph_data *glyph_data,
                   Data::Texture_data *texture_data,
                   Data::Text_draw_call_data *text_draw_call,
                   Data::Transform_data *transform_data,
                   Data::Entity_data *entity_data,
                   opContext *ctx,
                   opBuffer *buf)
{
  // Generate the string data
  Data::data_lock(font_data);
  Data::data_lock(texture_data);
  Data::data_lock(glyph_data);
  Data::data_lock(text_draw_call);

  // Build new string.
  {
    Text::Font_bitmap font_bitmap;
    Data::font_get_font_bitmap(font_data, font_id, &font_bitmap);
  
    float advance = 0;
    float line = 0;
    int data_ptr = 0;
    
    float str_tex_data[512];
    memset(str_tex_data, 0, sizeof(str_tex_data));

    for(int i = 0; i < strlen(glyph_arr); ++i)
    {
      char curr_char = glyph_arr[i];
      
      if(curr_char == '\n')
      {
        line -= font_bitmap.ascent / 512;
        advance = 0;
        continue;
      }
    
      // Find advance
      for(int j = 0; j < glyph_data->size; ++j)
      {
        if(glyph_arr[i] == glyph_data->keys[j])
        {
          str_tex_data[data_ptr++] = j;
          str_tex_data[data_ptr++] = advance;
          str_tex_data[data_ptr++] = line;
          
          advance += glyph_data->field_character[j].advance[0];
        }
      }
    }
    
//    Ogl::Texture glyph_texture;
//    Ogl::Texture glyph_metrics_texture;
    opID glyph_texture;
    opID glyph_metrics_texture;
    stbtt_fontinfo info;
    {
      Data::font_get_font_face(font_data, font_id, &info);
      Data::font_get_glyph_texture_id(font_data, font_id, &glyph_texture);
      Data::font_get_metric_texture_id(font_data, font_id, &glyph_metrics_texture);
      
//      Data::texture_get_texture(texture_data, texture_id, &glyph_texture);
//      Data::texture_get_texture(texture_data, glyph_metrics_texture_id, &glyph_metrics_texture);
    }
    
    const math::transform transform = Entity_detail::get_transform(this_id, entity_data, transform_data);
    const math::mat4 world_mat = math::transform_get_world_matrix(transform);
    
    // If we don't have a draw call insert one.
    if(!Data::text_draw_call_exists(text_draw_call, this_id))
    {
      Data::text_draw_call_push(text_draw_call, this_id);
    }
    
    if(Data::text_draw_call_exists(text_draw_call, this_id))
    {
      opTextureDesc desc;
      desc.format = opPixelFormat_RGB32F;
      desc.dimention = opDimention_ONE;
      desc.width = data_ptr;
      
      const opID str_text_id = opBufferTextureCreate(ctx, buf, &str_tex_data, &desc);
      opBufferExec(ctx, buf);
    
//      Ogl::Texture string_texture;
//      Ogl::texture_create_1d(&string_texture, sizeof(float) * data_ptr, GL_RGB32F, &str_tex_data);
    
      ::Text_renderer::Draw_call dc;
      memcpy(dc.world_matrix, &world_mat, sizeof(world_mat));
      
      dc.texture       = glyph_texture;
      dc.glyph_metrics = glyph_metrics_texture;
      dc.string_info   = str_text_id;
      dc.string_size   = strlen(glyph_arr);

      Data::text_draw_call_set_draw_call(text_draw_call, this_id, &dc);
    }
  }
  
  Data::data_unlock(text_draw_call);
  Data::data_unlock(glyph_data);
  Data::data_unlock(texture_data);
  Data::data_unlock(font_data);
}


} // ns