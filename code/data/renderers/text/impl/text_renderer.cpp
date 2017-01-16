#include "../text_renderer.hpp"
#include "draw_call.hpp"
#include "font_data.hpp"
#include "text_renderer_data.hpp"
#include <math/math.hpp>
#include <utilities/assert.hpp>
#include <utilities/logging.hpp>
#include <utilities/file.hpp>
#include <utilities/string_helpers.hpp>
#include <utilities/directory.hpp>
#include <utilities/bits.hpp>
#include <utilities/id32.hpp>
#include <op/op.hpp>

#include <systems/text/font_bitmap.hpp>


namespace {


opID text_shader = 0;
opID text_shader_wvp = 0;
opID text_shader_metrics = 0;
opID text_shader_details = 0;
opID text_shader_map = 0;
opID text_rasterizer = 0;
opID text_blendmode = 0;


} // anon ns


namespace Data {
namespace Text_renderer {


Text_renderer_data *
initialize(opContext *ctx,
           opBuffer *buf,
           const size_t inital_font_size_hint,
           const size_t inital_glyph_size_hint,
           const size_t inital_string_size_hint,
           const util::malloc_fn malloc_fn,
           const util::realloc_fn realloc_fn,
           const util::free_fn free_fn)
{
  // -- Param Check -- //
  UTIL_ASSERT(ctx);
  UTIL_ASSERT(buf);
  UTIL_ASSERT(inital_font_size_hint);
  UTIL_ASSERT(inital_glyph_size_hint);
  UTIL_ASSERT(inital_string_size_hint);
  UTIL_ASSERT(malloc_fn);
  
  // -- We do this so we don't have to expose the internals -- //
  void *data = malloc_fn(sizeof(Text_renderer_data));
  Text_renderer_data *renderer = new(data) Text_renderer_data();

  // -- Initialize buffers and get memory -- //
  {
    bool setup = true;
    
    // Fonts
    setup &= util::buffer::init(&renderer->font_data,
                                sizeof(Font),
                                inital_font_size_hint,
                                malloc_fn, realloc_fn, free_fn);
    
    // Glyphs
    setup &= util::buffer::init(&renderer->glyph_keys,
                                sizeof(uint32_t),
                                inital_glyph_size_hint,
                                malloc_fn, realloc_fn, free_fn);
    
    setup &= util::buffer::init(&renderer->glyph_data,
                                sizeof(Data::Character),
                                inital_glyph_size_hint,
                                malloc_fn, realloc_fn, free_fn);
    
    // Strings
    setup &= util::buffer::init(&renderer->string_keys,
                                sizeof(uint32_t),
                                inital_string_size_hint,
                                malloc_fn, realloc_fn, free_fn);
    
    setup &= util::buffer::init(&renderer->string_data,
                                sizeof(char),
                                inital_string_size_hint * 32,
                                malloc_fn, realloc_fn, free_fn);
    
    setup &= util::buffer::init(&renderer->draw_calls,
                                sizeof(Draw_call),
                                inital_string_size_hint,
                                malloc_fn, realloc_fn, free_fn);
    
    if(!setup)
    {
      util::buffer::destroy(&renderer->font_data);
      util::buffer::destroy(&renderer->glyph_keys);
      util::buffer::destroy(&renderer->glyph_data);
      util::buffer::destroy(&renderer->string_keys);
      util::buffer::destroy(&renderer->string_data);
      util::buffer::destroy(&renderer->draw_calls);
      
      LOG_ERROR("Failed to setup Font Renderer");
      
      return nullptr;
    }
  }
  
  // -- Initialize shader -- //
  if(text_shader == 0)
  {
    char core_text_shd_path[MAX_FILE_PATH_SIZE];
    {
      memset(core_text_shd_path, 0, sizeof(core_text_shd_path));
      strcat(core_text_shd_path, util::dir::resource_path());
      strcat(core_text_shd_path, "assets/shaders/core_text.ogl");
    }
    
    char shader_code[1 << 12];
    {
      memset(shader_code, 0, sizeof(shader_code));
      util::file::get_contents_from_file(core_text_shd_path, shader_code, sizeof(shader_code));
      assert(strlen(shader_code));
    }
    
    char vs_code[1024];
    {
      memset(vs_code, 0, sizeof(vs_code));
      util::get_text_between_tags("/* VERT_SHD */", "/* VERT_SHD */", shader_code, vs_code, sizeof(vs_code));
    }

    char gs_code[1 << 12];
    {
      memset(gs_code, 0, sizeof(gs_code));
      util::get_text_between_tags("/* GEO_SHD */", "/* GEO_SHD */", shader_code, gs_code, sizeof(gs_code));
    }
    
    char fs_code[1024];
    {
      memset(fs_code, 0, sizeof(fs_code));
      util::get_text_between_tags("/* FRAG_SHD */", "/* FRAG_SHD */", shader_code, fs_code, sizeof(fs_code));
    }
    
    opShaderDesc shader_desc;
    {
      memset(&shader_desc, 0, sizeof(shader_desc));
      
      text_shader = opBufferShaderCreate(
        ctx,
        buf,
        "Text Shader",
        vs_code,
        gs_code,
        fs_code,
        &shader_desc
      );
    }
    
    opShaderDataDesc shader_data_wvp_desc;
    {
      memset(&shader_data_wvp_desc, 0, sizeof(shader_data_wvp_desc));
      
      text_shader_wvp = opBufferShaderDataCreate
      (
        ctx,
        buf,
        text_shader,
        "uni_wvp_mat",
        &shader_data_wvp_desc
      );
    }
    
    opShaderDataDesc shader_data_metrics_desc;
    {
      memset(&shader_data_metrics_desc, 0, sizeof(shader_data_metrics_desc));
      
      text_shader_metrics = opBufferShaderDataCreate
      (
        ctx,
        buf,
        text_shader,
        "uni_metrics",
        &shader_data_metrics_desc
      );
    }
    
    opShaderDataDesc shader_data_details_desc;
    {
      memset(&shader_data_details_desc, 0, sizeof(shader_data_details_desc));
      
      text_shader_details = opBufferShaderDataCreate
      (
        ctx,
        buf,
        text_shader,
        "uni_string",
        &shader_data_details_desc
      );
    }
    
    opShaderDataDesc shader_data_map_desc;
    {
      memset(&shader_data_map_desc, 0, sizeof(shader_data_map_desc));
      
      text_shader_map = opBufferShaderDataCreate
      (
        ctx,
        buf,
        text_shader,
        "uni_map_01",
        &shader_data_map_desc
      );
    }
    
    opRasterizerDesc rasterizer_desc;
    {
      memset(&rasterizer_desc, 0, sizeof(rasterizer_desc));
      
      rasterizer_desc.cull_face     = opCullFace_BACK;
      rasterizer_desc.primitive     = opPrimitive_POINT;
      rasterizer_desc.winding_order = opWindingOrder_CCW;
      
      text_rasterizer = opBufferRasterizerCreate(
        ctx,
        buf,
        &rasterizer_desc
      );
    }
    
    opBlendDesc blend_desc;
    {
      memset(&blend_desc, 0, sizeof(blend_desc));
      
      blend_desc.enabled = opBool_TRUE;
      
      text_blendmode = opBufferBlendCreate(ctx, buf, &blend_desc);
    }

    // Load up the resources.
    opBufferExec(ctx, buf);
    
    // Did we load everything?
    {
      UTIL_ASSERT(shader_desc.status == opStatus_VALID);
      UTIL_ASSERT(shader_data_wvp_desc.status == opStatus_VALID);
      UTIL_ASSERT(shader_data_metrics_desc.status == opStatus_VALID);
      UTIL_ASSERT(shader_data_details_desc.status == opStatus_VALID);
      UTIL_ASSERT(shader_data_map_desc.status == opStatus_VALID);
      UTIL_ASSERT(rasterizer_desc.status == opStatus_VALID);
      UTIL_ASSERT(blend_desc.status == opStatus_VALID);
    }
  }
  
  LOG_INFO("Renderer setup successfully");
  
  // -- Finished Setup -- //
  return renderer;
}


namespace {


uint32_t
create_glyph_id(const uint16_t font_id, const uint16_t codepoint)
{
  return util::bits_pack(font_id, codepoint);
}


} // ns


void
set_draw_call(Text_renderer_data *renderer,
              const uint32_t id,
              const uint32_t font_id,
              const char *glyph_arr,
              opContext *ctx,
              opBuffer *buf)
{
  LOG_TODO_ONCE("Check font extists first");
  const uint32_t font_index = font_id - 1;
  
  Font *font = &((Font*)util::buffer::bytes(&renderer->font_data))[font_index];
  
  // Find and add missing glyphs
  {
//    Data::data_lock(font_data);
//    Data::data_lock(texture_data);
//    Data::data_lock(glyph_data);
    
    // Get various font infos
    opID glyph_texture = font->glyph_texture_id;
    opID glyph_metrics_texture = font->metrics_texture_id;
    stbtt_fontinfo info = font->font_info;
    Data::Font_bitmap font_bitmap = font->bitmap_info;
    
    for(int i = 0; i < strlen(glyph_arr); ++i)
    {
      const int codepoint = glyph_arr[i];
      
      // If code point exists skip
      const uint32_t glyph_id = create_glyph_id(font_id, codepoint);
      
      const uint32_t *glyph_ids = (uint32_t*)util::buffer::bytes(&renderer->glyph_keys);
      const size_t glyph_ids_count = util::buffer::size(&renderer->glyph_keys);
      
      if(util::id32::linear_search(glyph_id, glyph_ids, glyph_ids_count))
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
      opBufferTextureUpdate(
        ctx,
        buf,
        glyph_texture,
        font_bitmap.bitmap_offset[0],
        font_bitmap.bitmap_offset[1],
        glyph_width,
        glyph_height,
        glyph_bitmap);
      
      opBufferExec(ctx, buf);
      
      int kern;
      kern = stbtt_GetCodepointKernAdvance(&info, codepoint, glyph_arr[i + 1]);
      
      stbtt_FreeBitmap(glyph_bitmap, nullptr);
      
      // Add glyph info
      util::buffer::push(&renderer->glyph_keys);
      util::buffer::push(&renderer->glyph_data);
      
      uint32_t *last_id = (uint32_t*)util::buffer::last(&renderer->glyph_keys);
      *last_id = glyph_id;
      
      Data::Character *char_info = (Data::Character*)util::buffer::last(&renderer->glyph_data);
      
      char_info->size[0] = glyph_width;
      char_info->size[1] = glyph_height;
      
      char_info->offset[0] = x_offset;
      char_info->offset[1] = y_offset;

      char_info->advance[0] = (math::to_float(advance) * font_bitmap.scale) / math::to_float(font_bitmap.bitmap_size[0]);
      char_info->advance[1] = (font_bitmap.line_height + font_bitmap.decsent) * font_bitmap.scale;
      
      char_info->uv[0] = math::to_float(font_bitmap.bitmap_offset[0]) / math::to_float(font_bitmap.bitmap_size[0]);
      char_info->uv[1] = math::to_float(font_bitmap.bitmap_offset[1]) / math::to_float(font_bitmap.bitmap_size[1]);
      
      char_info->st[0] = char_info->uv[0] + (math::to_float(glyph_width) / math::to_float(font_bitmap.bitmap_size[0]));
      char_info->st[1] = char_info->uv[1] + (math::to_float(glyph_height) / math::to_float(font_bitmap.bitmap_size[1]));
      
      // We can now add the advance
      font_bitmap.bitmap_offset[0] += bitmap_advance + 2;
      
//      Data::font_glyph_push(glyph_data, glyph_id);
//      Data::font_glyph_set_character(glyph_data, glyph_id, &char_info);

      opBufferTextureUpdate(
        ctx,
        buf,
        glyph_metrics_texture,
        0,
        util::buffer::size(&renderer->glyph_data) * 5,
        util::buffer::bytes(&renderer->glyph_data)
      );
      opBufferExec(ctx, buf);
    }
    

      // Mess! I think this is above!
    // Update the metrics information
    {
////      Data::font_set_font_bitmap(font_data, font_id, &font_bitmap);
//      font->bitmap_info = font_bitmap;
//      
//      opID metric_texture_id = font->metrics_texture_id;
////      Data::font_get_metric_texture_id(font_data, font_id, &metric_texture_id);
//      
////      Ogl::Texture metrics_texture;
////      Data::texture_get_texture(texture_data, metric_texture_id, &metrics_texture);
//    
//      Ogl::texture_update_texture_1d(&metrics_texture, 0, glyph_data->size * (sizeof(float) * 4) * 4, glyph_data->field_character);
//      
//      opBufferTextureUpdate(
//        ctx,
//        buf,
//        metric_texture_id,
//        0,
//        glyph_data->size * (sizeof(float) * 4) * 4,
//        nullptr
//      );
    }
    
  }
  
  // Generate the string data
  {
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
        memset(&desc, 0, sizeof(desc));
        desc.format = opPixelFormat_RGB32F;
        desc.dimention = opDimention_ONE;
        desc.width = data_ptr * sizeof(float);
        
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
}



void
remove_draw_call(Text_renderer_data *renderer,
                 const uint32_t id)
{
  
}


void
render(Text_renderer_data *renderer,
       const float view_proj[16],
       opContext *ctx,
       opBuffer *buf)
{
  
}


} // ns
} // ns