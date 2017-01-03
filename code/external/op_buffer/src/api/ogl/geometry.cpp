#ifdef OP_BUFFER_API_OGL


#include "geometry.hpp"
#include "details/ogl_common.hpp"
#include "details/error_code_to_text.hpp"
#include "details/enum_to_string.hpp"
#include "details/op_common.hpp"
#include "details/op_to_ogl.hpp"
#include "descs/descs.hpp"


namespace op {
namespace ogl {


void
geometry_create(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Cmd Data -- //
  command::cmd_geometry_create *cmd(
    reinterpret_cast<command::cmd_geometry_create*>(data)
  );

  // -- Last Bind Cache -- //
  if(context->geometry_currently_bound &&
    context->geometry_last_bind == cmd->geometry_id)
  {
    return;
  }

  context->geometry_currently_bound = true;
  context->geometry_last_bind       = cmd->geometry_id;

  // -- Get Geom Data -- //
  ogl::geometry_desc *internal_desc(
    &context->geometry_descs[cmd->geometry_id]
  );

  // -- Generate VBO -- //
  GLuint buffer = 0;
  glGenBuffers(1, &buffer);

  if(buffer)
  {
    const GLenum useage   = op_access_to_gl_draw(cmd->desc->access);
    const GLsizeiptr size = cmd->sizeof_data;

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, size, cmd->data, useage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // -- Update Internal Desc -- //
    internal_desc->element_count    = static_cast<uint32_t>(cmd->element_count);
    internal_desc->vertex_buffer_id = buffer;

    // -- Extra Logging -- //
    #ifdef OP_BUFFER_LOG_INFO
    context->log("[");
    context->log("  Geometry Buffer Created");
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
    context->log("Op Error - Geometry Buffer did not load");
  }
  #endif

  // -- Update in_out desc -- //
  {
    cmd->desc->status = op_status_from_gl_id(internal_desc->vertex_buffer_id);
  }

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code = glGetError();

  if(err_code)
  {
    context->log(
      "GL Error - VBO Create %d : %s",
      err_code,
      get_error_msg(err_code)
    );
  }
  #endif
}


void
geometry_update(context_data *context, void *data)
{
}


void
geometry_destroy(context_data *context, void *data)
{
}


void
geometry_bind(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //
  command::cmd_geometry_bind *cmd(
    reinterpret_cast<command::cmd_geometry_bind*>(data)
  );

  ogl::geometry_desc *context_geo_desc(
    &context->geometry_descs[cmd->geometry_id]
  );

  ogl::vertex_format_desc *context_vf_desc(
    &context->vertex_format_descs[context->vertex_format_last_bind]
  );

  ogl::shader_desc *context_shd_desc(
    &context->shader_descs[context->shader_last_bound]
  );

  // -- Bind Geometry -- //
  glBindBuffer(GL_ARRAY_BUFFER, context_geo_desc->vertex_buffer_id);

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code_buff = glGetError();

  if(err_code_buff)
  {
    context->log(
      "GL Error - Binding VBO %d : %s",
      err_code_buff,
      get_error_msg(err_code_buff)
    );
  }
  #endif

  // -- Vertex Format -- //
  {
    for(uint32_t i = 0; i < context_vf_desc->number_of_attributes; ++i)
    {
      const GLint NOT_USED = -1;
      const attr_desc *attrib = &context_vf_desc->attributes[i];

      const GLint index = glGetAttribLocation(context_shd_desc->program, attrib->name);

      if(index != NOT_USED)
      {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index,
                              static_cast<GLint>(attrib->size),
                              attrib->type,
                              GL_FALSE,
                              context_vf_desc->stride,
                              (void*)attrib->pointer);
      }
    }

    // -- Extra Check -- //
    #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
    const GLenum err_code_vert = glGetError();

    if(err_code_vert)
    {
      context->log(
        "GL Error - Applying Vertex Attrs %d : %s",
        err_code_vert,
        get_error_msg(err_code_vert)
      );
    }
    #endif
  }
}


} // ns
} // ns


#endif // api guard
