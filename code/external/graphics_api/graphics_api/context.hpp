#ifndef CONTEXT_INCLUDED_DED3635F_7F6A_43F3_A4B4_86130BB75291
#define CONTEXT_INCLUDED_DED3635F_7F6A_43F3_A4B4_86130BB75291


#include "gfx_fwd.hpp"
#include "gfx_common.hpp"


namespace Graphics_api {


class Context
{
public:

  explicit            Context();
  
  // ** Textures ** //
  
  uint32_t            texture_create(Texture_desc *desc, const void *data);
  void                texture_update(const uint32_t texture_id, const Envelope *env, const void *data);
  void                texture_desc(const uint32_t texture_id, Texture_desc *out_desc);
  
  // ** Geometry ** //
  
  
  // ** Shaders ** //
  
  uint32_t            shader_create(Shader_desc *desc, const char *filename);
  uint32_t            shader_create(Shader_desc *desc, const char *vs_code, const char *gs_code, const char *fs_code);
  void                shader_desc(const uint32_t shader_id, Shader_desc *desc);
  
  // ** Shader Uniforms ** //
  
  uint32_t            data_create(Data_desc *desc, const uint32_t shader_id, const char *name);
  
  // ** Rasterizer ** //
  
  // ** Vertex Format ** //
  
  // ** Execute ** //
  
  void                exec();
  
private:

  struct  Impl;
  Impl    *m_impl;
  
};


} // ns


#endif // inc guard