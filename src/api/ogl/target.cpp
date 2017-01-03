#ifdef OP_BUFFER_API_OGL


#include "device.hpp"
#include "details/ogl_common.hpp"
#include "details/error_code_to_text.hpp"
#include "details/op_common.hpp"
#include "details/op_to_ogl.hpp"
#include "descs/descs.hpp"


namespace op {
namespace ogl {


void
target_create(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //
  command::cmd_target_create *target_info = reinterpret_cast<command::cmd_target_create*>(data);
  ogl::target_desc *desc = &context->target_descs[target_info->target_id];

  // -- Set Details -- //
  desc->clear_red   = target_info->desc->clear_red_color;
  desc->clear_green = target_info->desc->clear_green_color;
  desc->clear_blue  = target_info->desc->clear_blue_color;
}


void
target_update(context_data *context, void *data)
{

}


void
target_destroy(context_data *context, void *data)
{

}


void
target_clear(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Info -- //
  command::cmd_target_create *target_info = reinterpret_cast<command::cmd_target_create*>(data);
  ogl::target_desc *desc = &context->target_descs[target_info->target_id];

  // -- Clear Target -- //
  glClearColor(desc->clear_red, desc->clear_green, desc->clear_blue, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code = glGetError();

  if(err_code)
  {
    context->log(
      "GL Error - Target Clear %d : %s",
      err_code,
      get_error_msg(err_code)
    );
  }
  #endif
}


void
target_bind(context_data *context, void *data)
{

}


} // ns
} // ns


#endif // ogl guard
