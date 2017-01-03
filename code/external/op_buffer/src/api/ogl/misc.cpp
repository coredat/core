#ifdef OP_BUFFER_API_OGL


#include "misc.hpp"
#include "details/ogl_common.hpp"
#include "details/error_code_to_text.hpp"
#include "details/op_common.hpp"
#include "details/op_to_ogl.hpp"
#include "descs/descs.hpp"


namespace op {
namespace ogl {


void
viewport_set(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //
  command::cmd_viewport_bind *cmd(
    reinterpret_cast<command::cmd_viewport_bind*>(data)
  );

  // -- Set GL -- //
  glViewport(cmd->start_x, cmd->start_y, cmd->end_x, cmd->end_y);

  // Don't need to log or gl err check this -- //
}


} // ns
} // ns


#endif // inc guard
