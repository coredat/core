#ifndef SHADER_UTILS_INCLUDED_7BF0A0B7_ECBD_41B6_8845_EE44E138E066
#define SHADER_UTILS_INCLUDED_7BF0A0B7_ECBD_41B6_8845_EE44E138E066


#include <core/common/core_fwd.hpp>


namespace Core {
namespace Shader_utils {


/*!
  Fullbright Shader
  Ignores all lighting information available.
*/
Core::Shader
get_fullbright_shader();


/*!
  Fixed Directional Light Shader
  Has a single hardcoded directional light.
*/
Core::Shader
get_fixed_directional_light_shader();


} // ns
} // ns


#endif // inc guard