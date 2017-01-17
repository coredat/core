#ifdef OP_BUFFER_API_OGL


#include "vertex_format.hpp"
#include "details/ogl_common.hpp"
#include "details/error_code_to_text.hpp"
#include "details/enum_to_string.hpp"
#include "details/op_common.hpp"
#include "details/op_to_ogl.hpp"
#include "descs/descs.hpp"


namespace {


GLenum
gl_type(const uint8_t type)
{
  switch(type)
  {
    case(opType_FLOAT):   return GL_FLOAT;        break;
    case(opType_FLOAT2):  return GL_FLOAT;        break;
    case(opType_FLOAT3):  return GL_FLOAT;        break;
    case(opType_FLOAT4):  return GL_FLOAT;        break;

    case(opType_INT):     return GL_INT;          break;
    case(opType_INT2):    return GL_INT;          break;
    case(opType_INT3):    return GL_INT;          break;
    case(opType_INT4):    return GL_INT;          break;

    // case(op::type::UINT):    return GL_UNSIGNED_INT;  break;
  }

  assert(false); // shouldn't have got here.
  return 0; // For waring message.
}


} // anon ns


namespace op {
namespace ogl {


void
vertex_fmt_create(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //
  command::cmd_input_create *cmd(
    reinterpret_cast<command::cmd_input_create*>(data)
  );

  ogl::vertex_format_desc *internal_desc(
    &context->vertex_format_descs[cmd->vertex_fmt_id]
  );
  assert(internal_desc);
  memset(internal_desc, 0, sizeof(ogl::vertex_format_desc));

  // -- Create format -- //
  for(decltype(cmd->desc_count) i = 0; i < cmd->desc_count; ++i)
  {
    const opVertexDesc *attr = &cmd->desc[i];
    attr_desc *out_attr = &internal_desc->attributes[i];

    strcpy(&out_attr->name[0], &attr->name[0]);
    out_attr->size         = static_cast<uint32_t>(op::ogl::element_count(attr->type));
    out_attr->type         = gl_type(attr->type);
    out_attr->pointer      = internal_desc->stride;
    internal_desc->stride += (op::ogl::element_count(attr->type) * sizeof(gl_type(attr->type)));

    internal_desc->number_of_attributes = i + 1;
    internal_desc->number_of_elements += op::ogl::element_count(attr->type);
  }

  // -- Extra Logging -- //
  #ifdef OP_BUFFER_LOG_INFO
  context->log("[");
  context->log("  Vertex Format");
  context->log("  --");
  for(decltype(cmd->desc_count) i = 0; i < cmd->desc_count; ++i)
  {
    const attr_desc *attr = &internal_desc->attributes[i];
    context->log("  %s: %s x %d", attr->name, get_type_name(attr->type), attr->size);
  }
  context->log("]");
  #endif
}


void
vertex_fmt_destroy(context_data *context, void *data)
{

}


void
vertex_fmt_bind(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //
  command::cmd_input_bind *cmd(
    reinterpret_cast<command::cmd_input_bind*>(data)
  );

  // -- Set Last Vertex Format Bind -- //
  context->vertex_format_currently_bound = true;
  context->vertex_format_last_bind = cmd->vertex_fmt_id;
}


}
}


#endif // api guard
