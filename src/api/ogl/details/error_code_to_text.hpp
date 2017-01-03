#ifdef OP_BUFFER_API_OGL
#ifndef ERROR_CODE_TO_TEXT_INCLUDED_AE4D2765_9022_4823_9E23_4B35ED23B19B
#define ERROR_CODE_TO_TEXT_INCLUDED_AE4D2765_9022_4823_9E23_4B35ED23B19B


#include "ogl_common.hpp"


namespace op {
namespace ogl {


constexpr char gl_invalid_enum[]                  = "GL_INVALID_ENUM";
constexpr char gl_invalid_value[]                 = "GL_INVALID_VALUE";
constexpr char gl_invalid_operation[]             = "GL_INVALID_OPERATION";
constexpr char gl_out_of_memory[]                 = "GL_OUT_OF_MEMORY";
constexpr char gl_invalid_framebuffer_operation[] = "GL_INVALID_FRAMEBUFFER_OPERATION";
constexpr char gl_unknown_error[]                 = "UNKNOWN_GL_ERROR";

#ifdef OGL_HAS_CONTEXT_LOST_ERROR
constexpr char gl_context_lost[]                  = "GL_CONTEXT_LOST";
#endif


inline const char *
get_error_msg(const GLenum error_code)
{
  switch(error_code)
  {
    case(GL_INVALID_ENUM):                  return gl_invalid_enum;
    case(GL_INVALID_VALUE):                 return gl_invalid_value;
    case(GL_INVALID_OPERATION):             return gl_invalid_operation;
    case(GL_OUT_OF_MEMORY):                 return gl_out_of_memory;
    case(GL_INVALID_FRAMEBUFFER_OPERATION): return gl_invalid_framebuffer_operation;

    #ifdef OGL_HAS_CONTEXT_LOST_ERROR
    case(GL_CONTEXT_LOST):                  return gl_context_lost;
    #endif

    default:                                return gl_unknown_error;
  }
}


} // ns
} // ns


#endif // inc guard
#endif // ogl api
