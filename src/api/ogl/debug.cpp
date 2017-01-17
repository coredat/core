#ifdef OP_BUFFER_API_OGL


#include "debug.hpp"
#include "details/ogl_common.hpp"
#include "details/error_code_to_text.hpp"
#include "details/enum_to_string.hpp"
#include "details/op_common.hpp"
#include "details/op_to_ogl.hpp"
#include "descs/descs.hpp"


namespace op {
namespace ogl {


void
debug_marker_push(context_data *context, void *data)
{
  #ifdef OGL_HAS_DEBUG_MARKER
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Cmd Data -- //
  command::cmd_debug_marker_push *cmd(
    reinterpret_cast<command::cmd_debug_marker_push*>(data)
  );

  // -- Push Debug Marker -- //
  glPushGroupMarkerEXT(strlen(cmd->name), cmd->name);

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code = glGetError();

  if(err_code)
  {
    context->log(
      "GL Error - Push Group Marker %d : %s",
      err_code,
      get_error_msg(err_code)
    );
  }
  #endif

  #endif
}


void
debug_marker_pop(context_data *context, void *data)
{
  #ifdef OGL_HAS_DEBUG_MARKER
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Cmd Data -- //
  command::cmd_debug_marker_pop *cmd(
    reinterpret_cast<command::cmd_debug_marker_pop*>(data)
  );

  // -- Pop Debug Marker -- //
  glPopGroupMarkerEXT();

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code = glGetError();

  if(err_code)
  {
    context->log(
      "GL Error - Pop Group Marker %d : %s",
      err_code,
      get_error_msg(err_code)
    );
  }
  #endif

  #endif
}


} // ns
} // ns


#endif // api guard
