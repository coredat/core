#ifndef SHADER_DESC_INCLUDED_C6A23CF1_F882_4A4C_BE6C_5E91E22A7F75
#define SHADER_DESC_INCLUDED_C6A23CF1_F882_4A4C_BE6C_5E91E22A7F75


#include "gfx_common.hpp"


namespace Graphics_api {


struct Shader_desc
{
  uint32_t    number_of_uniforms  = 0;
  uint32_t    number_of_samplers  = 0;
  bool        complete            = false;
  
  uintptr_t   platform_handle     = 0;
  uintptr_t   internal_handle     = 0;
};


} // ns


#endif // inc guard