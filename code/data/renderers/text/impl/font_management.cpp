#include "../text_renderer.hpp"
#include "text_renderer_data.hpp"
#include "font_data.hpp"
#include <op/op.hpp>
#include <math/math.hpp>
#include <utilities/buffer.hpp>
#include <utilities/logging.hpp>
#include <utilities/file.hpp>
#include <utilities/string.hpp>


namespace Data {
namespace Text_renderer {


uint32_t
add_font(Text_renderer_data *renderer,
         const char *filename,
         opContext *ctx,
         opBuffer *buf)
{
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
  opID glyphs_texture_id = 0;
  opID metrics_texture_id = 0;
  {
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
  }
  
  // Font info
  Data::Font_bitmap font_bitmap;
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
  lib::buffer::push(&renderer->font_data);
  const uint32_t font_id = lib::buffer::size(&renderer->font_data);
  {
    const uint32_t index = font_id - 1;
    
    Font *font = &((Font*)lib::buffer::bytes(&renderer->font_data))[index];
  
    memset(font->name, 0, sizeof(font->name));
    lib::string::filename_from_path(filename, font->name, sizeof(font->name));
    
    font->glyph_texture_id = glyphs_texture_id;
    font->metrics_texture_id = metrics_texture_id;
    font->font_info = info;
    font->bitmap_info = font_bitmap;
  }
  
  return font_id;
}


} // ns
} // ns
