#include "context.hpp"
#include "texture_desc.hpp"
#include "shader_desc.hpp"
#include "data_desc.hpp"
#include "utils/shader_utils.hpp"

// Platform API includes

#include "ogl/ogl_texture.hpp"
#include "ogl/ogl_shader.hpp"
#include "ogl/ogl_shader_uniform.hpp"


namespace Graphics_api {


struct Context::Impl
{
  Texture_desc  *texture_descs  = nullptr;
  uint32_t       texture_count  = 0;
  
  Shader_desc   *shader_descs = nullptr;
  uint32_t       shader_count = 0;
  
  Data_desc     *data_descs = nullptr;
  uint32_t       data_count = 0;
};


Context::Context()
: m_impl(new Impl)
{
  m_impl->texture_descs = new Texture_desc[2048];
  memset(m_impl->texture_descs, 0, sizeof(Texture_desc) * 2048);

  m_impl->shader_descs = new Shader_desc[64];
  memset(m_impl->shader_descs, 0, sizeof(Shader_desc) * 64);
  
  m_impl->data_descs = new Data_desc[128];
  memset(m_impl->data_descs, 0, sizeof(Data_desc) * 128);
}

  
// ** Textures ** //


uint32_t
Context::texture_create(Texture_desc *in_out_desc,
                        const void *data)
{
  assert(in_out_desc);
  
  Ogl::texture_create(in_out_desc, data);
  in_out_desc->internal_handle = m_impl->texture_count + 1;
  
  Texture_desc *new_desc = &m_impl->texture_descs[m_impl->texture_count++];
  
  memcpy(new_desc, in_out_desc, sizeof(Texture_desc));
  
  return m_impl->texture_count;
}


void
Context::texture_update(const uint32_t texture_id,
                        const Graphics_api::Envelope *env,
                        const void *data)
{
  assert(texture_id);
  
  Texture_desc desc;
  texture_desc(texture_id, &desc);
  
  Ogl::texture_update(&desc,
                      env->x_from,
                      env->y_from,
                      env->z_from,
                      env->x_to,
                      env->y_to,
                      env->z_to,
                      data);
}


void
Context::texture_desc(const uint32_t texture_id,
                      Texture_desc *out_desc)
{
  memcpy(out_desc, &m_impl->texture_descs[texture_id - 1], sizeof(Texture_desc));
}


// ** Shader Data ** //


uint32_t
Context::shader_create(Shader_desc *desc,
                       const char *filename)
{
  auto debug_code = Graphics_api::Util::shader_code_from_tagged_file(filename);
  
  return shader_create(desc,
                       debug_code.vs_code.c_str(),
                       debug_code.gs_code.c_str(),
                       debug_code.ps_code.c_str());
}


uint32_t
Context::shader_create(Shader_desc *desc,
                       const char *vs_code,
                       const char *gs_code,
                       const char *fs_code)
{
  // Param check
  assert(desc);
  
  Ogl::shader_create(desc, vs_code, gs_code, fs_code);
  
  /*
    Add shader to internal data
  */
  if(desc->platform_handle)
  {
    desc->internal_handle = m_impl->shader_count + 1;
    
    memcpy(&m_impl->shader_descs[m_impl->shader_count], desc, sizeof(Shader_desc));
    
    m_impl->shader_count++;
  }
  
  return m_impl->shader_count;
}


void
Context::shader_desc(const uint32_t shader_id,
                     Shader_desc *desc)
{
  
}


uint32_t
Context::data_create(Data_desc *desc, const uint32_t shader_id, const char *name)
{
  // Param check
  assert(desc);
  assert(name);
  
  /*
    Find shader.
  */
  Shader_desc shd_desc;
  shader_desc(shader_id, &shd_desc);
  
  /*
    Find shader.
  */
  Ogl::shader_uniform_find(desc, &shd_desc, name);
}


void
Context::exec()
{
}


} // ns