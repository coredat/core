#include "ogl_common.hpp"


namespace Ogl {


/*!
  Checks glGetError, if an error is found it will push it out
  in the stream that was provided.
*/
void
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
void
error_clear()
{
  while(glGetError()) {}
}


} // ns