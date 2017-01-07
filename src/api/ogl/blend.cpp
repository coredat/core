#ifdef OP_BUFFER_API_OGL


#include "blend.hpp"
#include "details/ogl_common.hpp"
#include "details/error_code_to_text.hpp"
#include "details/enum_to_string.hpp"
#include "details/op_common.hpp"
#include "details/op_to_ogl.hpp"
#include "descs/descs.hpp"


namespace op {
namespace ogl {


void
blend_create(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //
  command::cmd_blend_create *cmd(
    reinterpret_cast<command::cmd_blend_create*>(data)
  );

  // -- Get Blend Data -- //
  ogl::blend_internal_desc *internal_desc(
    &context->blend_descs[cmd->blend_id]
  );
  assert(internal_desc);

  if(cmd->desc->enabled)
  {
    internal_desc->enabled = GL_TRUE;
    internal_desc->src_factor  = GL_SRC_ALPHA;
    internal_desc->dest_factor = GL_ONE_MINUS_SRC_ALPHA;
  }
  else
  {
    internal_desc->enabled = GL_FALSE;
  }
  
  // -- Update In - Out Desc -- //
  cmd->desc->status = opStatus_VALID;  

  // -- Extra Logging -- //
  #ifdef OP_BUFFER_LOG_INFO
  context->log("[");
  context->log("  Blend Mode Created");
  context->log("  --");
  context->log("  Blending Enabled: %s ", get_boolean_name(internal_desc->enabled));
  context->log("  Src Factor: %s", get_blend_mode_name(internal_desc->src_factor));
  context->log("  Dest Factor: %s", get_blend_mode_name(internal_desc->dest_factor));
  context->log("]");
  #endif

  // -- Extra Logging --//
  #ifdef OP_BUFFER_API_EXTRA_LOGGING
  context->log("Creating Blending Mode\n");
  #endif
}


void
blend_update(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //

}


void
blend_destroy(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //

}


void
blend_bind(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //
  command::cmd_blend_bind *cmd(
    reinterpret_cast<command::cmd_blend_bind*>(data)
  );

  // -- Get Blend Data -- //
  ogl::blend_internal_desc *internal_desc(
    &context->blend_descs[cmd->blend_id]
  );
  assert(internal_desc);

  if(internal_desc->enabled == GL_TRUE)
  {
    glEnable(GL_BLEND);
    glBlendFunc(internal_desc->src_factor, internal_desc->dest_factor);
  }
  else
  {
    glDisable(GL_BLEND);
  }

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code = glGetError();

  if(err_code)
  {
    context->log(
      "GL Error - Texture Create %d : %s",
      err_code,
      get_error_msg(err_code)
    );
  }
  #endif
}


} // ns
} // ns


#endif // api guard
