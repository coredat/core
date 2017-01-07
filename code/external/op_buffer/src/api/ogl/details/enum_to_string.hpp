/*
  Logging Helpers.
  Converts a GLenum into a string.
  These are grouped together, its not ment to be a all encompasing list.
*/
#ifdef OP_BUFFER_API_OGL
#ifndef ENUM_TO_STRING_INCLUDED_CFE8E147_E3A6_4E47_A1C5_CE5AF98832D2
#define ENUM_TO_STRING_INCLUDED_CFE8E147_E3A6_4E47_A1C5_CE5AF98832D2


#include "ogl_common.hpp"


namespace op {
namespace ogl {


// -- Unknown -- //


constexpr char gl_unknown_enum[] = "UNKOWN GL IDENTIFIER";


// -- Boolean -- //


constexpr char gl_false[] = "GL_FALSE";
constexpr char gl_true[]  = "GL_TRUE";


inline const char *
get_boolean_name(const GLenum boolean)
{
  switch(boolean)
  {
    case(GL_TRUE):  return gl_true;
    case(GL_FALSE): return gl_false;

    default:
      return gl_unknown_enum;
  }
}


// -- Useage -- //


constexpr char gl_dynamic_draw[] = "GL_DYNAMIC_DRAW";
constexpr char gl_static_draw[]  = "GL_STATIC_DRAW";


inline const char *
get_useage_name(const GLenum use)
{
  switch(use)
  {
    case(GL_DYNAMIC_DRAW): return gl_dynamic_draw;
    case(GL_STATIC_DRAW): return gl_static_draw;

    default:
      return gl_unknown_enum;
  }
}


// -- Primitives -- //


constexpr char gl_prim_points[]      = "GL_POINTS";
constexpr char gl_prim_line_strip[]  = "GL_LINE_STRIP";
constexpr char gl_prim_line_loop[]   = "GL_LINE_LOOP";
constexpr char gl_prim_lines[]       = "GL_LINES";
constexpr char gl_prim_tri_strip[]   = "GL_TRIANGLE_STRIP";
constexpr char gl_prim_tri_fan[]     = "GL_TRIANGLE_FAN";
constexpr char gl_prim_tris[]        = "GL_TRIANGLES";
constexpr char gl_prim_quads[]       = "GL_QUADS,";


inline const char *
get_primitive_name(const GLenum prim)
{
  switch(prim)
  {
    case(GL_POINTS):         return gl_prim_points;
    case(GL_LINE_STRIP):     return gl_prim_line_strip;
    case(GL_LINE_LOOP):      return gl_prim_line_loop;
    case(GL_LINES):          return gl_prim_lines;
    case(GL_TRIANGLE_STRIP): return gl_prim_tri_strip;
    case(GL_TRIANGLE_FAN):   return gl_prim_tri_fan;
    case(GL_TRIANGLES):      return gl_prim_tris;

    #ifdef OGL_HAS_GL_QUADS
    case(GL_QUADS):          return gl_prim_quads;
    #endif

    default:
      return gl_unknown_enum;
  }
}


// -- CULL OPTIONS -- //


constexpr char gl_cull_back[]           = "GL_BACK";
constexpr char gl_cull_front[]          = "GL_FRONT";
constexpr char gl_cull_front_and_back[] = "GL_FRONT_AND_BACK";


inline const char *
get_cull_name(const GLenum cull)
{
  switch(cull)
  {
    case(GL_BACK):            return gl_cull_back;
    case(GL_FRONT):           return gl_cull_front;
    case(GL_FRONT_AND_BACK):  return gl_cull_front_and_back;

    default:
      return gl_unknown_enum;
  }
}


// -- Winding Order -- //


constexpr char gl_winding_ccw[] = "GL_CCW";
constexpr char gl_winding_cw[]  = "GL_CW";


inline const char *
get_winding_name(const GLenum winding)
{
  switch(winding)
  {
    case(GL_CCW): return gl_winding_ccw;
    case(GL_CW):  return gl_winding_cw;

    default:
      return gl_unknown_enum;
  }
}


// -- Pixel Format -- //


constexpr char gl_r8[]      = "GL_R8";
constexpr char gl_rg8[]     = "GL_RG8";
constexpr char gl_rgb8[]    = "GL_RGB8";
constexpr char gl_rgba8[]   = "GL_RGBA8";
constexpr char gl_r32f[]    = "GL_R32F";
constexpr char gl_rg32f[]   = "GL_RG32F";
constexpr char gl_rgb32f[]  = "GL_RGB32F";
constexpr char gl_rgba32f[] = "GL_RGBA32F";


inline const char *
get_pixel_format_name(const GLint format)
{
  switch(format)
  {
    #ifndef OGL_GLES2

    case(GL_R8):      return gl_r8;
    case(GL_RG8):     return gl_rg8;
    case(GL_RGB8):    return gl_rgb8;
    case(GL_RGBA8):   return gl_rgba8;
    case(GL_R32F):    return gl_r32f;
    case(GL_RG32F):   return gl_rg32f;
    case(GL_RGB32F):  return gl_rgb32f;
    case(GL_RGBA32F): return gl_rgba32f;

    #else

    case(GL_RGB):    return gl_rgb8;
    case(GL_RGBA):   return gl_rgba8;

    #endif

    default:
      return gl_unknown_enum;
  }
}


constexpr char gl_red[]     = "GL_RED";
constexpr char gl_rg[]      = "GL_RG";
constexpr char gl_rgb[]     = "GL_RGB";
constexpr char gl_rgba[]    = "GL_RGBA";


inline const char *
get_pixel_format_component_name(const GLenum format)
{
  switch(format)
  {
    #ifndef OGL_GLES2

    case(GL_RED):     return gl_red;
    case(GL_RG):      return gl_rg;

    #endif

    case(GL_RGB):     return gl_rgb;
    case(GL_RGBA):    return gl_rgba;

    default:
      return gl_unknown_enum;
  }
}


constexpr char gl_unsigned_byte[]   = "GL_UNSIGNED_BYTE";
constexpr char gl_unsigned_int[]    = "GL_UNSIGNED_INT";
constexpr char gl_float[]           = "GL_FLOAT";
constexpr char gl_float_vec2[]      = "GL_FLOAT_VEC2";
constexpr char gl_float_vec3[]      = "GL_FLOAT_VEC3";
constexpr char gl_float_vec4[]      = "GL_FLOAT_VEC4";
constexpr char gl_float_mat2[]      = "GL_FLOAT_MAT2";
constexpr char gl_float_mat3[]      = "GL_FLOAT_MAT3";
constexpr char gl_float_mat4[]      = "GL_FLOAT_MAT4";
constexpr char gl_sampler_1d[]      = "GL_SAMPLER_1D";
constexpr char gl_sampler_2d[]      = "GL_SAMPLER_2D";
constexpr char gl_sampler_3d[]      = "GL_SAMPLER_3D";


inline const char *
get_type_name(const GLenum type)
{
  switch(type)
  {
    case(GL_UNSIGNED_BYTE):     return gl_unsigned_byte;
    case(GL_UNSIGNED_INT):      return gl_unsigned_int;
    case(GL_FLOAT):             return gl_float;
    case(GL_FLOAT_VEC2):        return gl_float_vec2;
    case(GL_FLOAT_VEC3):        return gl_float_vec3;
    case(GL_FLOAT_VEC4):        return gl_float_vec4;
    case(GL_FLOAT_MAT2):        return gl_float_mat2;
    case(GL_FLOAT_MAT3):        return gl_float_mat3;
    case(GL_FLOAT_MAT4):        return gl_float_mat4;
    case(GL_SAMPLER_2D):        return gl_sampler_2d;
    #ifndef OGL_GLES2
    case(GL_SAMPLER_1D):        return gl_sampler_1d;
    case(GL_SAMPLER_3D):        return gl_sampler_3d;
    #endif

    default:
      return gl_unknown_enum;
  }
}


// -- Texture Dimention -- //


constexpr char gl_texture_1D[] = "GL_TEXTURE_1D";
constexpr char gl_texture_2D[] = "GL_TEXTURE_2D";
constexpr char gl_texture_3D[] = "GL_TEXTURE_3D";


inline const char *
get_texture_dimention_name(const GLenum tex)
{
  switch(tex)
  {
    case(GL_TEXTURE_2D):        return gl_texture_2D;

    #ifndef OGL_GLES2
    case(GL_TEXTURE_1D):        return gl_texture_1D;
    case(GL_TEXTURE_3D):        return gl_texture_3D;
    #endif

    default:
      return gl_unknown_enum;
  }
}


// -- Texture Filter -- //


constexpr char gl_texture_wrap_mode_clamp_edge[]        = "GL_CLAMP_TO_EDGE";
constexpr char gl_texture_wrap_mode_repeat[]            = "GL_REPEAT";
constexpr char gl_texture_wrap_mode_mirror_repeat[]     = "GL_MIRRORED_REPEAT";

#ifdef OGL_HAS_CLAMP_TO_BORDER
constexpr char gl_texture_wrap_mode_clamp_border[]      = "GL_CLAMP_TO_BORDER";
#endif

#ifdef OGL_GL4
constexpr char gl_texture_wrap_mode_mirror_clamp_edge[] = "GL_MIRROR_CLAMP_TO_EDGE";
#endif


inline const char *
get_texture_wrap_mode(const GLint mode)
{
  switch(mode)
  {
    case(GL_CLAMP_TO_EDGE):         return gl_texture_wrap_mode_clamp_edge;
    case(GL_REPEAT):                return gl_texture_wrap_mode_repeat;
    case(GL_MIRRORED_REPEAT):       return gl_texture_wrap_mode_mirror_repeat;

    #ifdef OGL_HAS_CLAMP_TO_BORDER
    case(GL_CLAMP_TO_BORDER):       return gl_texture_wrap_mode_clamp_border;
    #endif

    #ifdef OGL_GL4
    case(GL_MIRROR_CLAMP_TO_EDGE):  return gl_texture_wrap_mode_mirror_clamp_edge;
    #endif

    default:
      return gl_unknown_enum;
  }
}

constexpr char gl_texture_filter_nearest[]        = "GL_NEAREST";
constexpr char gl_texture_filter_linear[]         = "GL_LINEAR";
constexpr char gl_texture_filter_mip_linear[]     = "GL_LINEAR_MIPMAP_LINEAR";

inline const char *
get_texture_filter_mode(const GLenum mode)
{
  switch(mode)
  {
    case(GL_NEAREST):               return gl_texture_filter_nearest;
    case(GL_LINEAR):                return gl_texture_filter_linear;
    case(GL_LINEAR_MIPMAP_LINEAR):  return gl_texture_filter_mip_linear;

    default:
      return gl_unknown_enum;
  }
}


} // ns
} // ns


#endif // inc guard
#endif // api guard
