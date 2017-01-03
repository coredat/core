#ifdef OP_BUFFER_API_OGL


#include "index.hpp"
#include "details/op_common.hpp"
#include "details/ogl_common.hpp"
#include "details/op_to_ogl.hpp"
#include "details/error_code_to_text.hpp"
#include "details/enum_to_string.hpp"
#include "descs/descs.hpp"


namespace op {
namespace ogl {


void
index_create(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //
  command::cmd_index_create *cmd(
    reinterpret_cast<command::cmd_index_create*>(data)
  );

  ogl::index_internal_desc *internal_desc(
    &context->index_descs[cmd->index_id]
  );

  // -- Generate IBO -- //
  GLuint buffer = 0;

  glGenBuffers(1, &buffer);

  if(buffer)
  {
    const GLenum useage   = op_access_to_gl_draw(cmd->desc->access);
    const GLsizeiptr size = (GLsizeiptr)cmd->data_bytes;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, cmd->data, useage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // -- Update Internal Desc -- //
    internal_desc->index_buffer_id = buffer;
    internal_desc->element_count   = cmd->element_count;

    #ifdef OP_BUFFER_LOG_INFO
    context->log("[");
    context->log("  Index Buffer Created");
    context->log("  --");
    context->log("  Elements: %d", internal_desc->element_count);
    context->log("  Useage: %s", get_useage_name(useage));
    context->log("  GL ID: %d", buffer);
    context->log("]");
    #endif
  }
  #ifdef OP_BUFFER_LOG_INFO
  else
  {
    context->log("OP Error - Index Buffer did not load");
  }
  #endif

  // -- Update in-out Desc -- //
  if(cmd->desc)
  {
    cmd->desc->status = op_status_from_gl_id(internal_desc->index_buffer_id);
  }

  // -- Error Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code = glGetError();
  if(err_code != GL_NO_ERROR)
  {
    context->log(
      "GL Error - Creating Index Buffer %d : %s",
      err_code,
      get_error_msg(err_code)
    );
  }
  #endif
}


void
index_update(context_data *context, void *data)
{

}


void
index_destroy(context_data *context, void *data)
{

}


void
index_bind(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Cmd Data -- //
  command::cmd_index_bind *cmd(
    reinterpret_cast<command::cmd_index_bind*>(data)
  );

  // -- Last Bind Cache -- //
  if(context->index_currently_bound &&
     context->index_last_bind == cmd->index_id)
  {
    return;
  }

  context->index_currently_bound = true;
  context->index_last_bind = cmd->index_id;

  // -- Get Index Data -- //
  ogl::index_internal_desc *internal_desc(
    &context->index_descs[cmd->index_id]
  );

  // -- Bind -- //
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, internal_desc->index_buffer_id);

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code = glGetError();

  if(err_code)
  {
    context->log(
      "GL Error - Index Buffer Bind %d : %s",
      err_code,
      get_error_msg(err_code)
    );
  }
  #endif
}


} // ns
} // ns


#endif // api guard
