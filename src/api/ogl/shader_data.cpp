#ifdef OP_BUFFER_API_OGL


#include "shader_data.hpp"
#include "details/ogl_common.hpp"
#include "details/error_code_to_text.hpp"
#include "details/enum_to_string.hpp"
#include "details/op_common.hpp"
#include "details/op_to_ogl.hpp"
#include "descs/descs.hpp"


namespace op {
namespace ogl {


void
shader_data_create(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //
  command::cmd_shader_data_create *cmd(
    reinterpret_cast<command::cmd_shader_data_create*>(data)
  );

  ogl::shader_data_desc *internal_desc(
    &context->shader_data_descs[cmd->shader_data_id]
  );
  assert(internal_desc);

  // Check we have a name to search for.
  if(cmd->name == nullptr || strlen(cmd->name) == 0)
  {
    context->log("No Shader uniform name provided");
    return;
  }

  // -- Loop through the uniforms looking for the uniform -- //
  const GLuint program_id = context->shader_descs[cmd->shader_id].program;

  for(size_t i = 0; i < context->uniform_desc_size; ++i)
  {
    const uniform_desc *uni_desc = &context->uniform_descs[i];

    if(uni_desc->program == program_id)
    {
      if(strcmp(uni_desc->name, cmd->name) == 0)
      {
        internal_desc->program  = program_id;
        internal_desc->index    = uni_desc->index;
        internal_desc->type     = uni_desc->type;
        internal_desc->count    = uni_desc->count;

        // -- Extra Logging -- //
        #ifdef OP_BUFFER_LOG_INFO
        context->log("[");
        context->log("  Shader Data");
        context->log("  --");
        context->log("  Name: %s", cmd->name);
        context->log("  Type: %s", get_type_name(internal_desc->type));
        context->log("]");
        #endif

        break;
      }
    }
  }
}


void
shader_data_destroy(context_data *context, void *data)
{
}


void
shader_data_bind(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //
  command::cmd_shader_data_bind *cmd(
    reinterpret_cast<command::cmd_shader_data_bind*>(data)
  );

  ogl::shader_data_desc *desc(
    &context->shader_data_descs[cmd->shader_data_id]
  );
  assert(desc);

  // -- Set Last Data Bind -- //
  context->shader_data_last_bound = cmd->shader_data_id;

  // -- Bind Data -- //
  switch(desc->type)
  {
    case(GL_FLOAT):
      glUniform1fv(desc->index, desc->count, (GLfloat*)cmd->data);
      break;
    case(GL_FLOAT_VEC2):
      glUniform2fv(desc->index, desc->count, (GLfloat*)cmd->data);
      break;
    case(GL_FLOAT_VEC3):
      glUniform3fv(desc->index, desc->count, (GLfloat*)cmd->data);
      break;
    case(GL_FLOAT_VEC4):
      glUniform4fv(desc->index, desc->count, (GLfloat*)cmd->data);
      break;
    case(GL_INT):
      glUniform1iv(desc->index, desc->count, (GLint*)cmd->data);
      break;
    case(GL_INT_VEC2):
      glUniform2iv(desc->index, desc->count, (GLint*)cmd->data);
      break;
    case(GL_INT_VEC3):
      glUniform3iv(desc->index, desc->count, (GLint*)cmd->data);
      break;
    case(GL_INT_VEC4):
      glUniform4iv(desc->index, desc->count, (GLint*)cmd->data);
      break;
    case(GL_BOOL):
      glUniform1iv(desc->index, desc->count, (GLint*)cmd->data);
      break;
    case(GL_BOOL_VEC2):
      glUniform2iv(desc->index, desc->count, (GLint*)cmd->data);
      break;
    case(GL_BOOL_VEC3):
      glUniform3iv(desc->index, desc->count, (GLint*)cmd->data);
      break;
    case(GL_BOOL_VEC4):
      glUniform4iv(desc->index, desc->count, (GLint*)cmd->data);
      break;
    case(GL_FLOAT_MAT2):
      glUniformMatrix2fv(desc->index,
                         desc->count,
                         GL_FALSE,
                         (GLfloat*)cmd->data);
      break;
    case(GL_FLOAT_MAT3):
      glUniformMatrix3fv(desc->index,
                         desc->count,
                         GL_FALSE,
                         (GLfloat*)cmd->data);
      break;
    case(GL_FLOAT_MAT4):
      glUniformMatrix4fv(desc->index,
                         desc->count,
                         GL_FALSE,
                         (GLfloat*)cmd->data);
      break;

    #ifndef OGL_GLES2_SAMPLERS
    case(GL_SAMPLER_1D):
    case(GL_SAMPLER_1D_ARRAY):
    case(GL_SAMPLER_1D_ARRAY_SHADOW):
    case(GL_SAMPLER_1D_SHADOW):
    {
      const size_t texture_id = static_cast<size_t>(cmd->data);
      const texture_desc *texture_desc = &context->texture_descs[texture_id];

      glActiveTexture(GL_TEXTURE0 + desc->index);
      glBindTexture(GL_TEXTURE_1D, texture_desc->texture_id);

      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

      break;
    }
    #endif

    case(GL_SAMPLER_2D):
    #ifndef OGL_GLES2_SAMPLERS
    case(GL_SAMPLER_2D_ARRAY):
    case(GL_SAMPLER_2D_ARRAY_SHADOW):
    case(GL_SAMPLER_2D_MULTISAMPLE):
    case(GL_SAMPLER_2D_MULTISAMPLE_ARRAY):
    case(GL_SAMPLER_2D_RECT):
    case(GL_SAMPLER_2D_RECT_SHADOW):
    case(GL_SAMPLER_2D_SHADOW):
    #endif
    {
      const size_t texture_id = static_cast<size_t>(cmd->data);
      const texture_desc *texture_desc(
        &context->texture_descs[texture_id]
      );

      const size_t texture_filter_id = context->texture_filtering_last_bind;
      const texture_filtering_internal_desc *texture_filter_desc(
        &context->texture_filtering_descs[texture_filter_id]
      );

      glActiveTexture(GL_TEXTURE0 + desc->index);
      glBindTexture(GL_TEXTURE_2D, texture_desc->texture_id);

      const GLint s_wrap(
        op_filtering_wrap_mode(texture_filter_desc->wrap_mode_width)
      );
      const GLint t_wrap(
        op_filtering_wrap_mode(texture_filter_desc->wrap_mode_width)
      );
      const GLint min_filter(
        op_filtering_min_mode(texture_filter_desc->filter_mode,
                              texture_desc->mips)
      );
      const GLint mag_filter(
        op_filtering_mag_mode(texture_filter_desc->filter_mode,
                              texture_desc->mips)
      );

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_wrap);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_wrap);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);

      break;
    }

    #ifndef OGL_GLES2_SAMPLERS
    case(GL_SAMPLER_3D):
    {
      assert(false); // Not impled yet.
      break;
    }
    #endif

    default:
      break;
  };

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code = glGetError();

  if(err_code)
  {
    context->log(
      "GL Error - Binding shader data %d : %s",
      err_code,
      get_error_msg(err_code)
    );
  }
  #endif
}


} // ns
} // ns


#endif // api guard
