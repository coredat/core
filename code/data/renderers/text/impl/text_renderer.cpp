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
#include <utilities/assert.hpp>
#include <op/op.hpp>

#include <systems/text/font_bitmap.hpp>


namespace {


opID text_shader          = 0;
opID text_shader_wvp      = 0;
opID text_shader_metrics  = 0;
opID text_shader_details  = 0;
opID text_shader_map      = 0;
opID text_rasterizer      = 0;
opID text_blendmode       = 0;


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
                                sizeof(uint64_t),
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


inline uint64_t
create_glyph_id(const uint32_t font_id, const uint32_t codepoint)
{
  return util::bits_pack(font_id, codepoint);
}


} // ns


void
set_draw_call(Text_renderer_data *renderer,
              const uint32_t id,
              const uint32_t font_id,
              const char *glyph_arr,
              const float world_matrix[16],
              opContext *ctx,
              opBuffer *buf)
{
  // -- Param Check -- //
  UTIL_ASSERT(renderer);
  UTIL_ASSERT(id);
  UTIL_ASSERT(font_id);
  UTIL_ASSERT(glyph_arr);
  UTIL_ASSERT(world_matrix);
  UTIL_ASSERT(ctx);
  UTIL_ASSERT(buf);


  // -- Check if we have the font -- //
  // Note: This isn't a great check but since we can't remove fonts its ok.
  if(font_id > util::buffer::size(&renderer->font_data))
  {
    LOG_ERROR("Font doesn't exist.");
    return;
  }


  // -- Add missing glyphs to the bitmap -- //
  
  Font *curr_font = nullptr;
  {
    stbtt_fontinfo info;
    Data::Font_bitmap *font_bitmap = nullptr;
    
    Font *fonts = (Font*)util::buffer::bytes(&renderer->font_data);
    
    const uint32_t font_index = font_id - 1;
    curr_font = &fonts[font_index];

    info        = curr_font->font_info;
    font_bitmap = &curr_font->bitmap_info;
  
    // Find and add missing glyphs
    const size_t string_len = strlen(glyph_arr);
    
    for(int i = 0; i < string_len; ++i)
    {
      const int codepoint = glyph_arr[i];
      
      if(codepoint == '\n')
      {
        continue;
      }
      
      // Check to see if the codepoint is already in the map.
      const uint64_t  glyph_id        = create_glyph_id(font_id, codepoint);
      const uint64_t *glyph_ids       = (uint64_t*)util::buffer::bytes(&renderer->glyph_keys);
      const size_t    glyph_ids_count = util::buffer::size(&renderer->glyph_keys);
      
      if(util::id::linear_search(glyph_id, glyph_ids, glyph_ids_count))
      {
        continue;
      }
      
      // Create a new glyph
      
      int glyph_width, glyph_height;
      int x_offset, y_offset;
      
      unsigned char * glyph_bitmap = stbtt_GetCodepointBitmap(
        &info,
        0,
        font_bitmap->scale,
        codepoint,
        &glyph_width,
        &glyph_height,
        &x_offset,
        &y_offset
      );

      int advance, left_side_bearing;
      stbtt_GetCodepointHMetrics(&info, codepoint, &advance, &left_side_bearing);
      
      const int bitmap_advance = glyph_width;
      const int width_needed = bitmap_advance + glyph_width;
      
      if(font_bitmap->bitmap_offset[0] + width_needed > font_bitmap->bitmap_size[0])
      {
        if(font_bitmap->bitmap_offset[1] + font_bitmap->line_height > font_bitmap->bitmap_size[1])
        {
          LOG_WARNING("Font map is full.");
          break;
        }
        
        font_bitmap->bitmap_offset[0] = 0;
        font_bitmap->bitmap_offset[1] += glyph_height;
      }

      opBufferExec(ctx, buf);
      opBufferTextureUpdate(
        ctx,
        buf,
        curr_font->glyph_texture_id,
        font_bitmap->bitmap_offset[0],
        font_bitmap->bitmap_offset[1],
        glyph_width,
        glyph_height,
        glyph_bitmap
      );
      
      opBufferExec(ctx, buf);
      
//      int kern;
//      kern = stbtt_GetCodepointKernAdvance(&info, codepoint, glyph_arr[i + 1]);
      
      stbtt_FreeBitmap(glyph_bitmap, nullptr);
      
      // Add glyph info
      util::buffer::push(&renderer->glyph_keys);
      util::buffer::push(&renderer->glyph_data);
      
      uint64_t *last_id = (uint64_t*)util::buffer::last(&renderer->glyph_keys);
      *last_id = glyph_id;
      
      Data::Character *char_info = (Data::Character*)util::buffer::last(&renderer->glyph_data);
      
      char_info->size[0] = glyph_width;
      char_info->size[1] = glyph_height;
      
      char_info->offset[0] = x_offset;
      char_info->offset[1] = y_offset;

      char_info->advance[0] = (math::to_float(advance) * font_bitmap->scale) / math::to_float(font_bitmap->bitmap_size[0]);
      char_info->advance[1] = (font_bitmap->line_height + font_bitmap->decsent) * font_bitmap->scale;
      
      char_info->uv[0] = math::to_float(font_bitmap->bitmap_offset[0]) / math::to_float(font_bitmap->bitmap_size[0]);
      char_info->uv[1] = math::to_float(font_bitmap->bitmap_offset[1]) / math::to_float(font_bitmap->bitmap_size[1]);
      
      char_info->st[0] = char_info->uv[0] + (math::to_float(glyph_width) / math::to_float(font_bitmap->bitmap_size[0]));
      char_info->st[1] = char_info->uv[1] + (math::to_float(glyph_height) / math::to_float(font_bitmap->bitmap_size[1]));
      
      // We can now add the advance
      font_bitmap->bitmap_offset[0] += bitmap_advance + 2;
      
      opBufferTextureUpdate(
        ctx,
        buf,
        curr_font->metrics_texture_id,
        0,
        util::buffer::size(&renderer->glyph_data) * 4,
        util::buffer::bytes(&renderer->glyph_data)
      );
      opBufferExec(ctx, buf);
    }
  }  // Add Missing Glyphs to font
  
//  printf("Glyphs: %d", util::buffer::size(&renderer->glyph_keys));
  
  
  // -- Build new string and Set/Update Draw call -- //
  {
    float advance = 0;
    float line    = 0;
    int data_ptr  = 0;

    const uint64_t *glyph_keys        = (uint64_t*)util::buffer::bytes(&renderer->glyph_keys);
    const size_t glyph_keys_size      = util::buffer::size(&renderer->glyph_keys);
    const Data::Character *glyph_data = (Data::Character*)util::buffer::bytes(&renderer->glyph_data);
    
    float str_tex_data[512];
    memset(str_tex_data, 0, sizeof(str_tex_data));
    
    const size_t number_of_chars = strlen(glyph_arr);
    
    for(int i = 0; i < number_of_chars; ++i)
    {
      char curr_char = glyph_arr[i];
      
      if(curr_char == '\n')
      {
        line -= curr_font->bitmap_info.ascent / 512;
        advance = 0;
        continue;
      }
    
      // Find advance
      for(int j = 0; j < glyph_keys_size; ++j)
      {
        const uint64_t glyph_id = create_glyph_id(font_id, glyph_arr[i]);
      
        if(glyph_id == glyph_keys[j])
        {
          str_tex_data[data_ptr++] = j;
          str_tex_data[data_ptr++] = advance;
          str_tex_data[data_ptr++] = line;
          str_tex_data[data_ptr++] = 123;
          
          advance += glyph_data[j].advance[0];
        }
      }
    }

    // -- Create/Update Draw Call -- //
    {
      // Check to see if it exists.
      Data::Draw_call *dc = nullptr;
      {
        size_t index = 0;
        
        const uint32_t *ids   = (uint32_t*)util::buffer::bytes(&renderer->string_keys);
        const size_t id_count = util::buffer::size(&renderer->string_keys);
        
        if(!util::id::linear_search(id, ids, id_count, &index))
        {
          util::buffer::push(&renderer->string_keys);
          util::buffer::push(&renderer->string_data);
          util::buffer::push(&renderer->draw_calls);
          
          index = util::buffer::size(&renderer->string_keys) - 1;
          uint32_t *last_id = (uint32_t*)util::buffer::last(&renderer->string_keys);
          
          *last_id = id;
        }
        
        Data::Draw_call *draw_calls = (Data::Draw_call*)util::buffer::bytes(&renderer->draw_calls);
        dc = &draw_calls[index];
        
        // Double check all sizes are equal //
        UTIL_ASSERT(
          util::buffer::size(&renderer->string_keys) ==
          util::buffer::size(&renderer->string_data) ==
          util::buffer::size(&renderer->draw_calls)
        );
      }
    
      // Create
      if(!dc->string_info)
      {
        opTextureDesc desc;
        memset(&desc, 0, sizeof(desc));
        
        desc.format    = opPixelFormat_RGBA32F;
        desc.dimention = opDimention_ONE;
        desc.width     = 512;
        
        dc->string_info = opBufferTextureCreate(ctx, buf, &str_tex_data, &desc);
      }
      // Update
      else
      {
        opBufferTextureUpdate(ctx, buf, dc->string_info, 0, 512, &str_tex_data);
      }
      
      opBufferExec(ctx, buf);
    
      memcpy(dc->world_matrix, world_matrix, sizeof(float) * 16);
      
      dc->texture       = curr_font->glyph_texture_id;
      dc->glyph_metrics = curr_font->metrics_texture_id;
      dc->string_size   = strlen(glyph_arr);
    }
  } // -- Build new string and Set/Update Draw call -- //
}



void
remove_draw_call(Text_renderer_data *renderer,
                 const uint32_t id)
{
  UTIL_ASSERT(false); // not impled yet.
}


void
render(Text_renderer_data *renderer,
       const float view_proj[16],
       opContext *ctx,
       opBuffer *buf)
{
  Draw_call *calls = (Draw_call*)util::buffer::bytes(&renderer->draw_calls);
  const size_t number_of_calls = util::buffer::size(&renderer->draw_calls);
  
  const math::mat4 view_proj_mat = math::mat4_init_with_array(view_proj);

  // opBuffer is delayed so we need to store the wvp mats here until opExec is called.
  for(uint32_t i = 0; i < number_of_calls; ++i)
  {
    const math::mat4 world_mat = math::mat4_init_with_array(calls[i].world_matrix);
    const math::mat4 scale = math::mat4_scale(math::vec3_init(10));
    const math::mat4 wvp_mat = math::mat4_multiply(scale, world_mat, view_proj_mat);
    math::mat4_to_array(wvp_mat, calls[i].wvp_matrix);
  }

  opContextResetStats(ctx);

  opBufferDebugMarkerPush(buf, "// -- [TEXT RENDERER] -- //");
  opBufferDeviceReset(buf);
  opBufferBlendBind(buf, text_blendmode);
  opBufferRasterizerBind(buf, text_rasterizer);
  opBufferShaderBind(buf, text_shader);
  
  for(uint32_t i = 0; i < number_of_calls; ++i)
  {
    opBufferDebugMarkerPush(buf, "// -- [RENDER STRING] -- //");
    opBufferShaderDataBind(buf, text_shader_map, calls[i].texture);
    opBufferShaderDataBind(buf, text_shader_metrics, calls[i].glyph_metrics);
    opBufferShaderDataBind(buf, text_shader_details, calls[i].string_info);
    opBufferShaderDataBind(buf, text_shader_wvp, (void*)&calls[i].wvp_matrix);
    opBufferRenderSubset(buf, 0, calls[i].string_size);
    opBufferDebugMarkerPop(buf);
  }
  
  opBufferDebugMarkerPop(buf);
  opBufferExec(ctx, buf);

//  return opContextDrawCallStats(ctx);
}


void
update_draw_call_matrix(Text_renderer_data *renderer,
                        const uint32_t id,
                        const float world[16])
{
  // -- Param Check -- //
  UTIL_ASSERT(renderer);
  UTIL_ASSERT(id);
  UTIL_ASSERT(world);

  // -- Find the record -- //
  Data::Draw_call *dc = nullptr;
  {
    size_t index = 0;

    const uint32_t *ids   = (uint32_t*)util::buffer::bytes(&renderer->string_keys);
    const size_t id_count = util::buffer::size(&renderer->string_keys);

    if(!util::id::linear_search(id, ids, id_count, &index))
    {
      LOG_WARNING("This entity has no text renderer attached.");
      return;
    }
    
    Data::Draw_call *draw_calls = (Data::Draw_call*)util::buffer::bytes(&renderer->draw_calls);
    dc = &draw_calls[index];
  }
  
  // -- Update It -- //
  {
    memcpy(dc->world_matrix, world, sizeof(float) * 16);
  }
}

} // ns
} // ns