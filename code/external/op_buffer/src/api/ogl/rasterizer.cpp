#ifdef OP_BUFFER_API_OGL


#include "render.hpp"
#include "details/ogl_common.hpp"
#include "details/error_code_to_text.hpp"
#include "details/enum_to_string.hpp"
#include "details/op_common.hpp"
#include "details/op_to_ogl.hpp"
#include "descs/descs.hpp"


namespace {


inline GLenum
convert_cull_face(uint32_t cull)
{
  switch(cull)
  {
    case(opCullFace_BACK):
      return GL_BACK;
    case(opCullFace_FRONT):
      return GL_FRONT;
    case(opCullFace_BOTH):
      return GL_FRONT_AND_BACK;
    case(opCullFace_NONE):
    default:
      return GL_FALSE;
  }
}


} // anon ns


namespace op {
namespace ogl {


void
rasterizer_create(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //
  command::cmd_rasterizer_create *cmd = reinterpret_cast<command::cmd_rasterizer_create*>(data);
  ogl::rasterizer_desc *internal_desc = &context->rasterizer_descs[cmd->rasterizer_id];
  assert(internal_desc);

  opRasterizerDesc *external_desc = cmd->desc;
  assert(external_desc);

  // -- Set Internal Desc -- //
  internal_desc->primitive     = op_primitive_to_gl_primitive(external_desc->primitive);
  internal_desc->winding_order = external_desc->winding_order == opWindingOrder_CCW ? GL_CCW : GL_CW;
  internal_desc->cull_face     = convert_cull_face(external_desc->cull_face);

  // -- Extra Logging -- //
  #ifdef OP_BUFFER_LOG_INFO
  context->log("[");
  context->log("  Rasterizer Created");
  context->log("  --");
  context->log("  Primitive: %s", get_primitive_name(internal_desc->primitive));
  context->log("  Winding Order: %s", get_winding_name(internal_desc->winding_order));
  context->log("  Culling: %s", get_cull_name(internal_desc->cull_face));
  context->log("]");
  #endif
}


void
rasterizer_update(context_data *context, void *data)
{

}


void
rasterizer_destroy(context_data *context, void *data)
{

}


void
rasterizer_bind(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Cmd Data -- //
  command::cmd_rasterizer_bind *cmd(
    reinterpret_cast<command::cmd_rasterizer_bind*>(data)
  );

  // -- Last Bind Cache -- //
  if(context->rasterizer_currently_bound &&
     context->rasterizer_last_bind == cmd->rasterizer_id)
  {
    return;
  }

  context->rasterizer_currently_bound = true;
  context->rasterizer_last_bind = cmd->rasterizer_id;

  // -- Get Rasterizer Data -- //
  ogl::rasterizer_desc *internal_desc(
    &context->rasterizer_descs[cmd->rasterizer_id]
  );
  assert(internal_desc);

  // Winding Order
  glFrontFace(internal_desc->winding_order);

  // CullFace
  if(internal_desc->cull_face != GL_FALSE)
  {
    glEnable(GL_CULL_FACE);
    glCullFace(internal_desc->cull_face);
  }
  else
  {
    glDisable(GL_CULL_FACE);
  }

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code = glGetError();

  if(err_code)
  {
    context->log(
      "GL Error - Rasterizer Bind %d : %s",
      err_code,
      get_error_msg(err_code)
    );
  }
  #endif
}


} // ns
} // ns



#endif // api guard
