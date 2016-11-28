#include "context.hpp"
#include "texture_desc.hpp"
#include "ogl/ogl_texture.hpp"


namespace Graphics_api {


struct Context::Impl
{
  Texture_desc  *texture_descs  = nullptr;
  uint32_t       texture_count  = 0;
};


Context::Context()
: m_impl(new Impl)
{
  m_impl->texture_descs = new Texture_desc[2048];
  memset(m_impl->texture_descs, 0, sizeof(Texture_desc) * 2048);
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
  out_desc = &m_impl->texture_descs[texture_id - 1];
}


void
Context::exec()
{
}


} // ns