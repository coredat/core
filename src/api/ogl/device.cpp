#ifdef OP_BUFFER_API_OGL


#include "device.hpp"
#include "details/op_common.hpp"
#include "details/ogl_common.hpp"
#include "details/error_code_to_text.hpp"
#include "descs/descs.hpp"


namespace {

#ifdef OGL_HAS_VAO
GLuint vao(0);
#endif

} // anon ns


namespace op {
namespace ogl {


void
device_init(context_data *context, void *data)
{
  // -- Initalize OGL -- //
  #ifdef OGL_HAS_VAO
  if(!vao)
  {
    glGenVertexArrays(1, &vao);
  }
  #endif

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code = glGetError();

  if(err_code)
  {
    context->log(
      "GL Error - Device Initalize %d : %s",
      err_code,
      get_error_msg(err_code)
    );
  }
  #endif

  // -- Reset -- //
  device_reset(context, data);
}


void
device_reset(context_data *context, void *data)
{
  glDisable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);
  glFrontFace(GL_CCW);

  glUseProgram(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  #ifdef OGL_HAS_VAO
  if(vao)
  {
    glBindVertexArray(vao);
  }
  #endif

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code = glGetError();

  if(err_code)
  {
    context->log(
      "GL Error - Device Reset %d : %s",
      err_code,
      get_error_msg(err_code)
    );
  }
  #endif
}


void
device_destroy(context_data *context, void *data)
{
  #ifdef OGL_HAS_VAO
  if(vao)
  {
    glBindVertexArray(vao);
    glDeleteVertexArrays(1, &vao);
  }
  #endif

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code = glGetError();

  if(err_code)
  {
    context->log(
      "GL Error - Device Destroy %d : %s",
      err_code,
      get_error_msg(err_code)
    );
  }
  #endif
}


} // ns
} // ns

#endif // ogl guard
