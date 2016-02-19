#ifndef COMMON_INCLUDED_57928899_C288_49E0_8694_6E85E36592EA
#define COMMON_INCLUDED_57928899_C288_49E0_8694_6E85E36592EA


#include <ostream>
#ifdef WIN32
#include <GL/glew.h>
#else
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif


namespace Ogl {


/*!
  Resets the gl states to the applications default.
*/
inline void
default_state()
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);
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