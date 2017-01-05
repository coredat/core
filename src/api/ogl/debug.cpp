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
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Cmd Data -- //
  command::cmd_debug_marker_push *cmd(
    reinterpret_cast<command::cmd_debug_marker_push*>(data)
  );

  // -- Push Debug Marker -- //
  glPushGroupMarkerEXT(strlen(cmd->name), cmd->name);
}


void
debug_marker_pop(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Cmd Data -- //
  command::cmd_debug_marker_pop *cmd(
    reinterpret_cast<command::cmd_debug_marker_pop*>(data)
  );

  // -- Pop Debug Marker -- //
  glPopGroupMarkerEXT();
}


} // ns
} // ns


#endif // api guard
