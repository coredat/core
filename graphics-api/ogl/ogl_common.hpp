#ifndef COMMON_INCLUDED_57928899_C288_49E0_8694_6E85E36592EA
#define COMMON_INCLUDED_57928899_C288_49E0_8694_6E85E36592EA


#include <ostream>
#ifdef WIN32
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

#include <iostream> // for error stream this needs to change.

namespace Ogl {


/*!
  Resets the gl states to the applications default.
*/
inline void
default_state()
{
  glDisable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);
  glFrontFace(GL_CCW);
}


/*!
  Sugar function to for clear color
*/
inline void
clear_color(const float r = 0,
            const float g = 0,
            const float b = 0,
            const float a = 0)
{
  glClearColor(r, g, b, a);
}


/*!
  Sugar function to clear attached buffer
  \param color If true clears the attached color buffer.
  \param depth If true clears the attached depth buffer.
*/
inline void
clear(const bool color, const bool depth)
{
  GLint clear = 0;
  
  if(color) { clear |= GL_COLOR_BUFFER_BIT; }
  if(depth) { clear |= GL_DEPTH_BUFFER_BIT; }

  glClear(clear);
}


/*!
  Initialize vao
*/
inline void
vao_init()
{
  static GLuint vao(0);
  
  if(!vao)
  {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
  }
}


/*!
  Checks glGetError, if an error is found it will push it out
  in the stream that was provided.
*/
inline void
error_check(const char* msg, std::ostream *stream)
{
  if(stream) // Check stream first, if none given we don't want to clear the error.
  {
    const auto err_id = glGetError();

    if(err_id != GL_NO_ERROR)
    {
      (*stream) << err_id << " " << msg << "\n";
    }
  }
}


/*!
  Clears any pending error.
*/
inline void
error_clear()
{
  while(glGetError()) {}
}


} // ns


#endif // inc guard