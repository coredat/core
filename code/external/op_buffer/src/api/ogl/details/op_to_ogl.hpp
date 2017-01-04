/*
  Helpers to convert from op types and gl types and back
*/
#ifdef OP_BUFFER_API_OGL
#ifndef OP_TO_OGL_INCLUDED_4EC281DE_5CCF_45A0_A805_131FCF928799
#define OP_TO_OGL_INCLUDED_4EC281DE_5CCF_45A0_A805_131FCF928799

// #include <OpenGL/gl3.h>

#include "ogl_common.hpp"


namespace op {
namespace ogl {


/*
  CPU Access Flags
  --
  Converts op::access to GL_X_DRAW
*/
inline GLenum
op_access_to_gl_draw(const uint8_t access)
{
  switch(access)
  {
    case(opAccess_STATIC):   return GL_STATIC_DRAW;
    case(opAccess_DYNAMIC):  return GL_DYNAMIC_DRAW;

    default:
      assert(false);
      return GL_STATIC_DRAW;
  }
}


/*
  Validity Check
  --
  If a GL int is valid (greater than zero) we return op::status flag.
*/
inline uint8_t
op_status_from_gl_id(const GLuint id)
{
  return id ? opStatus_VALID : opStatus_INVALID;
}


/*
  Primitive
*/
inline GLenum
op_primitive_to_gl_primitive(const uint8_t primitive)
{
  switch(primitive)
  {
    case(opPrimitive_POINT):          return GL_POINTS;
    case(opPrimitive_LINE):           return GL_LINES;
    case(opPrimitive_LINE_STRIP):     return GL_LINE_STRIP;
    case(opPrimitive_TRIANGLE):       return GL_TRIANGLES;
    case(opPrimitive_TRIANGLE_STRIP): return GL_TRIANGLE_STRIP;
    case(opPrimitive_TRIANGLE_FAN):   return GL_TRIANGLE_FAN;

    #ifdef OGL_HAS_GL_QUADS
    case(opPrimitive_QUAD):           return GL_QUADS;
    #endif

    default:
      assert(false);
      return opPrimitive_TRIANGLE;
  }
}


/*
  Pixel Format
*/
inline GLint
op_pixel_format_to_internal_format(const uint8_t format)
{
  switch(format)
  {
    #ifndef OGL_GLES2
    case(opPixelFormat_R8):       return GL_R8;
    case(opPixelFormat_RG8):      return GL_RG8;
    case(opPixelFormat_RGB8):     return GL_RGB8;
    case(opPixelFormat_RGBA8):    return GL_RGBA8;
    case(opPixelFormat_R32F):     return GL_R32F;
    case(opPixelFormat_RG32F):    return GL_RG32F;
    case(opPixelFormat_RGB32F):   return GL_RGB32F;
    case(opPixelFormat_RGBA32F):  return GL_RGBA32F;
    case(opPixelFormat_UNKNOWN):  return GL_RGB8;

    default:
      assert(false);
      return GL_RGBA8;

    #else
    case(opPixelFormat_R8):       return GL_RGB;
    case(opPixelFormat_RG8):      return GL_RGB;
    case(opPixelFormat_RGB8):     return GL_RGB;
    case(opPixelFormat_RGBA8):    return GL_RGBA;
    case(opPixelFormat_R32F):     return GL_RGB;
    case(opPixelFormat_RG32F):    return GL_RGB;
    case(opPixelFormat_RGB32F):   return GL_RGB;
    case(opPixelFormat_RGBA32F):  return GL_RGBA;
    case(opPixelFormat_UNKNOWN):  return GL_RGB;

    default:
      assert(false);
      return GL_RGBA;

    #endif
  }
}


/*
  Component Format
*/
inline GLenum
op_pixel_format_to_format(const uint8_t format)
{
  switch(format)
  {
    case(opPixelFormat_R8):
    case(opPixelFormat_R32F):
      #ifndef OGL_GLES2
      return GL_RED;
      #else
      return GL_RGB;
      #endif

    case(opPixelFormat_RG8):
    case(opPixelFormat_RG32F):
      #ifndef OGL_GLES2
      return GL_RG;
      #else
      return GL_RGB;
      #endif

    case(opPixelFormat_RGB8):
    case(opPixelFormat_RGB32F):
      return GL_RGB;

    case(opPixelFormat_RGBA8):
    case(opPixelFormat_RGBA32F):
      return GL_RGBA;

    default:
      assert(false);
      return GL_RGBA;
  }
}


/*
  Pixel Format Type
*/
inline GLenum
op_pixel_format_to_type(const uint8_t format)
{
  switch(format)
  {
    case(opPixelFormat_R8):       return GL_UNSIGNED_BYTE;
    case(opPixelFormat_RG8):      return GL_UNSIGNED_BYTE;
    case(opPixelFormat_RGB8):     return GL_UNSIGNED_BYTE;
    case(opPixelFormat_RGBA8):    return GL_UNSIGNED_BYTE;
    case(opPixelFormat_R32F):     return GL_FLOAT;
    case(opPixelFormat_RG32F):    return GL_FLOAT;
    case(opPixelFormat_RGB32F):   return GL_FLOAT;
    case(opPixelFormat_RGBA32F):  return GL_FLOAT;

    default:
      assert(false);
      return GL_UNSIGNED_BYTE;
  }
}


/*
  Texture Filtering Wrap Mode
  CLAMP,
  REPEAT,

*/
inline GLint
op_filtering_wrap_mode(const uint8_t wrap_mode)
{
  switch(wrap_mode)
  {
    case(opWrapMode_CLAMP):   return GL_CLAMP_TO_EDGE;
    case(opWrapMode_REPEAT):  return GL_REPEAT;

    default:
      assert(false);
      return GL_CLAMP_TO_EDGE;
  }
}


/*
  Texture Filtering Mode
  POINT,
  LINEAR,
  MIP_LINEAR,
*/
inline GLint
op_filtering_min_mode(const uint8_t filter_mode, const bool has_mips)
{
  switch(filter_mode)
  {
    case(opFilterMode_POINT):
      return GL_NEAREST;

    case(opFilterMode_BILINEAR):
      return GL_LINEAR;

    case(opFilterMode_TRILINEAR):
      return has_mips ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

    case(opFilterMode_ANISOTROPIC):
      return has_mips ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

    default:
      assert(false);
      return GL_NEAREST;
  }
}


inline GLint
op_filtering_mag_mode(const uint8_t filter_mode, const bool has_mips)
{
  switch(filter_mode)
  {
    case(opFilterMode_POINT):       return GL_NEAREST;
    case(opFilterMode_BILINEAR):    return GL_LINEAR;
    case(opFilterMode_TRILINEAR):   return GL_LINEAR;
    case(opFilterMode_ANISOTROPIC): return GL_LINEAR;//GL_TEXTURE_MAX_ANISOTROPY_EXT;

    default:
      assert(false);
      return GL_NEAREST;
  }
}


constexpr uint8_t
element_count(const uint8_t element)
{
  switch(element)
  {
    case(opType_FLOAT):     return 1;
    case(opType_FLOAT2):    return 2;
    case(opType_FLOAT3):    return 3;
    case(opType_FLOAT4):    return 4;

    case(opType_INT):       return 1;
    case(opType_INT2):      return 2;
    case(opType_INT3):      return 3;
    case(opType_INT4):      return 4;
    
    case(opType_MAT2):      return 4;
    case(opType_MAT3):      return 9;
    case(opType_MAT4):      return 16;

    default:         return 0;
  }
}


constexpr uint8_t
ogl_type_to_op(const GLenum type)
{
  switch(type)
  {
    case(GL_FLOAT):           return opType_FLOAT;
    case(GL_FLOAT_VEC2):      return opType_FLOAT2;
    case(GL_FLOAT_VEC3):      return opType_FLOAT3;
    case(GL_FLOAT_VEC4):      return opType_FLOAT4;

    case(GL_INT):             return opType_INT;
    case(GL_INT_VEC2):        return opType_INT2;
    case(GL_INT_VEC3):        return opType_INT3;
    case(GL_INT_VEC4):        return opType_INT4;
    
    case(GL_FLOAT_MAT2):      return opType_MAT2;
    case(GL_FLOAT_MAT3):      return opType_MAT3;
    case(GL_FLOAT_MAT4):      return opType_MAT4;
  
    default:
      return 0;
  }
}


} // ns
} // ns


#endif // api guard
#endif // api guard
