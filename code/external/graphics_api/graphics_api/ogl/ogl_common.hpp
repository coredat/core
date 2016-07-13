#ifndef COMMON_INCLUDED_57928899_C288_49E0_8694_6E85E36592EA
#define COMMON_INCLUDED_57928899_C288_49E0_8694_6E85E36592EA


// Use these alot so to keep includes down
// I've included very comment headers here.
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <cstring>


#ifdef _WIN32
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

#include <utilities/logging.hpp>

#define LOG_GL_ERROR(err_code, msg) \
LOG_TODO("Need to be able to pass params into logging") \
LOG_ERROR(msg) \

#define LOG_GL_WARNING(msg) LOG_WARNING(msg)
#define LOG_GL_PARAM_ERROR(msg) LOG_ERROR(msg)

#ifndef NDEBUG
#define OGL_EXTRA_ERROR_CHECKS
#define OGL_EXTRA_PARAM_CHECKS
#endif

namespace Ogl {


/*!
  Resets the gl states to the applications default.
*/
void
default_state();


/*!
  Sugar function to for clear color
*/
void
clear_color(const float r = 0,
            const float g = 0,
            const float b = 0,
            const float a = 0);


/*!
  Sugar function to clear attached buffer
  \param color If true clears the attached color buffer.
  \param depth If true clears the attached depth buffer.
*/
void
clear(const bool color, const bool depth);


/*!
  Initialize vao
*/
void
vao_init();


/*!
  Clears any pending error.
*/
void
error_clear();


} // ns


#endif // inc guard