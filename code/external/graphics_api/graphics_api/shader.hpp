#ifndef SHADER_INCLUDED_1074CF4E_A888_4A60_9C2A_CF87CF7C5AB0
#define SHADER_INCLUDED_1074CF4E_A888_4A60_9C2A_CF87CF7C5AB0


#include "gfx_common.hpp"
#include "ogl/ogl_shader.hpp"


namespace Graphics_api {


struct Shader : public API_TYPE::Shader
{
};


inline void       shader_init();
inline void       shader_get_uniform();


} // ns


#endif // inc guard