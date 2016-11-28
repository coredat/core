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
  
  // ** Execute ** //
  
  void                exec();
  
private:

  struct  Impl;
  Impl    *m_impl;
  
};


} // ns


#endif // inc guard