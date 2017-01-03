#ifdef OP_BUFFER_API_OGL


#include "shader.hpp"
#include "details/ogl_common.hpp"
#include "details/error_code_to_text.hpp"
#include "details/op_common.hpp"
#include "details/op_to_ogl.hpp"
#include "descs/descs.hpp"


namespace op {
namespace ogl {


namespace {


bool
compile_shader(const char *shader_code,
               const GLenum shader_type,
               GLuint *out_shd_id,
               GLchar *output_log,
               const GLsizei log_output_size)
{
  // -- Param Check -- //
  if(shader_code == nullptr || strlen(shader_code) == 0)
  {
    return true; // It didn't 'fail' to compile
  }

  // Compile the shader.
  {
    const GLuint shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_code, NULL);
    glCompileShader(shader_id);

    *out_shd_id = shader_id;
  }

  // Log
  {
    GLint log_length = 0;
    glGetShaderiv(*out_shd_id, GL_INFO_LOG_LENGTH, &log_length);

    if(log_length > 1)
    {
      // So we can add what shader type to the log.
      constexpr GLsizei output_header = 20;
      if(log_length + output_header < log_output_size)
      {
        GLsizei log_offset = 0;

        switch(shader_type)
        {
          case(GL_VERTEX_SHADER):
            strcat(output_log, "GL_VERTEX_SHADER\n");
            log_offset += strlen("GL_VERTEX_SHADER\n");
            break;

          #ifdef OGL_HAS_GEOMETRY_SHADER
          case(GL_GEOMETRY_SHADER):
            strcat(output_log, "GL_GEOMETRY_SHADER\n");
            log_offset += strlen("GL_GEOMETRY_SHADER\n");
            break;
          #endif

          case(GL_FRAGMENT_SHADER):
            strcat(output_log, "GL_FRAGMENT_SHADER\n");
            log_offset += strlen("GL_FRAGMENT_SHADER\n");
            break;

          default:
            strcat(output_log, "GL_UNKNOWN_SHADER\n");
            log_offset += strlen("GL_UNKNOWN_SHADER\n");
        }

        glGetShaderInfoLog(*out_shd_id, log_length, 0, &output_log[log_offset]);
      }

      // Did it compile
      GLint is_compiled = false;
      glGetShaderiv(*out_shd_id, GL_COMPILE_STATUS, &is_compiled);

      if(is_compiled == GL_FALSE)
      {
        return false;
      }
    }
  }

  return true;
}


bool
link_shaders(const GLuint vs_id,
             const GLuint gs_id,
             const GLuint fs_id,
             GLuint *out_prog_id,
             GLchar *output_log,
             const GLsizei log_output_size)
{
  const GLuint new_program = glCreateProgram();
  *out_prog_id = new_program;

  #ifdef OGL_EXTRA_ERROR_CHECKS
  const GLenum program_compile_err_code = glGetError();
  if(program_compile_err_code != GL_NO_ERROR)
  {
    LOG_GL_ERROR(program_compile_err_code, "Compiling program");
  }
  #endif

  glAttachShader(*out_prog_id, vs_id);

  if(gs_id)
  {
    glAttachShader(*out_prog_id, gs_id);
  }

  glAttachShader(*out_prog_id, fs_id);
  glLinkProgram(*out_prog_id);

  #ifdef OGL_EXTRA_ERROR_CHECKS
  const GLenum link_shader_err_code = glGetError();
  if(link_shader_err_code != GL_NO_ERROR)
  {
    LOG_GL_ERROR(link_shader_err_code, "Compiling program");
  }
  #endif

  // Log
  {
    GLint log_length(0);
		glGetProgramiv(*out_prog_id, GL_INFO_LOG_LENGTH, &log_length);

    if(log_length)
    {
      GLchar output_log[512];
      memset(output_log, 0, sizeof(output_log));

      glGetProgramInfoLog(*out_prog_id, log_length, 0, output_log);

      // LOG_WARNING(output_log);
    }
  }

  #ifdef OGL_EXTRA_ERROR_CHECKS
  const GLenum err = glGetError();
  if(err != GL_NO_ERROR)
  {
    // LOG_GL_ERROR(err, "Creating Program");
  }
  #endif

  // Did it link
  GLint is_linked;
  glGetProgramiv(*out_prog_id, GL_LINK_STATUS, &is_linked);

  if(!is_linked) // Failed
  {
    // LOG_ERROR("Failed to link shader program.")

    /*
      Need to clean up
    */

    return false;
  }

  #ifdef OGL_EXTRA_ERROR_CHECKS
  const GLenum shader_complete_err = glGetError();
  if(shader_complete_err != GL_NO_ERROR)
  {
    // LOG_GL_ERROR(shader_complete_err, "Creating Program");
  }
  #endif

  return true;
}


} // anon ns


void
shader_create(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //
  command::cmd_shader_create *cmd = reinterpret_cast<command::cmd_shader_create*>(data);
  ogl::shader_desc *internal_desc = &context->shader_descs[cmd->shader_id];
  assert(internal_desc);

  // -- Compile And Link Shaders -- //
  GLuint vs_id  = 0;
  GLuint gs_id  = 0;
  GLuint fs_id  = 0;
  GLuint shd_id = 0;
  GLsizei uniform_data_count = 0;
  GLsizei sampler_count = 0;

  bool compile_link_success = true;
  {
    constexpr GLsizei output_log_size = 512;
    GLchar output_log[output_log_size];
    memset(output_log, 0, sizeof(output_log));

    // Compile shaders //
    const bool vs_compiled = compile_shader(
      cmd->vs,
      GL_VERTEX_SHADER,
      &vs_id,
      output_log,
      output_log_size
    );

    if(strlen(output_log))
    {
      context->log((char*)output_log);
      memset(output_log, 0, sizeof(output_log));
    }

    #ifdef OGL_HAS_GEOMETRY_SHADER
    const bool gs_compiled = compile_shader(
      cmd->gs,
      GL_GEOMETRY_SHADER,
      &gs_id,
      output_log,
      output_log_size
    );

    if(strlen(output_log))
    {
      context->log((char*)output_log);
      memset(output_log, 0, sizeof(output_log));
    }
    #else
    const bool gs_compiled = true;
    #endif

    const bool ps_compiled = compile_shader(
      cmd->ps,
      GL_FRAGMENT_SHADER,
      &fs_id,
      output_log,
      output_log_size
    );

    if(strlen(output_log))
    {
      context->log((char*)output_log);
      memset(output_log, 0, sizeof(output_log));
    }

    // -- Link Shaders -- //
    if(vs_compiled && gs_compiled && ps_compiled)
    {
      if(!link_shaders(
        vs_id,
        gs_id,
        fs_id,
        &shd_id,
        output_log,
        output_log_size - strlen(output_log))
      )
      {
        compile_link_success = false;
      }
    }
    else
    {
      compile_link_success = false;
    }

    // -- Any GL Logging -- //
    if(strlen(output_log))
    {
      context->log((char*)output_log);
    }
  }

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_comp_link = glGetError();

  if(err_comp_link)
  {
    context->log(
      "Compliing and linking shader %d : %s",
      err_comp_link,
      get_error_msg(err_comp_link)
    );
  }
  #endif

  // -- Get uniforms and samplers -- //
  {
    GLint uniform_count, uniform_length;
    glGetProgramiv(shd_id, GL_ACTIVE_UNIFORMS, &uniform_count);
    glGetProgramiv(shd_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &uniform_length);

    // -- Extra Check -- //
    #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
    const GLenum err_active_uniforms = glGetError();

    if(err_active_uniforms)
    {
      context->log(
        "Getting Active uniforms for shader %d, %d : %s",
        shd_id,
        err_active_uniforms,
        get_error_msg(err_active_uniforms)
      );
    }
    #endif

    // Allocate space for uniforms.
    internal_desc->uniforms = (uniform_desc*)context->alloc(uniform_count * sizeof(uniform_desc));

    // Allocate space for array of names.
    const uint32_t number_of_chars = (uniform_count * uniform_length) + uniform_count;
    internal_desc->uniform_names = (char*)context->alloc(sizeof(char) * number_of_chars);
    memset(internal_desc->uniform_names, 0, sizeof(char) * number_of_chars);

    uint32_t name_count = 0;

    GLchar curr_uni_name[512];

    for(GLint i = 0; i < uniform_count; ++i)
    {
      // -- Extra Logging -- //
      #ifdef OP_BUFFER_LOG_INFO
      context->log("Shader has %d active uniforms:", uniform_count);
      #endif

      // Reset the buffer.
      memset(curr_uni_name, 0, sizeof(curr_uni_name));

      GLenum gl_type = 0;
      GLint length   = 0;
      GLint size     = 0;

      glGetActiveUniform(shd_id,
                         i,
                         uniform_length,
                         &length,
                         &size,
                         &gl_type,
                         curr_uni_name);

      strcat(&internal_desc->uniform_names[name_count], curr_uni_name);
      name_count += strlen(&internal_desc->uniform_names[name_count]);
      name_count += 1; // We want to skip over the null terminator

      // -- Extra Check -- //
      #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
      const GLenum err_code = glGetError();

      if(err_code)
      {
        context->log(
          "GL Error - Getting Active Uniforms  %d : %s",
          err_code,
          get_error_msg(err_code)
        );
      }
      #endif

      // Make sure we are not overflowing.
      assert(name_count < number_of_chars);

      // Is sampler?
      #ifdef OGL_GLES2_SAMPLERS
      if((gl_type >= GL_SAMPLER_2D) && (gl_type <= GL_SAMPLER_2D))
      #else
      if((gl_type >= GL_SAMPLER_1D) && (gl_type <= GL_SAMPLER_2D_RECT_SHADOW))
      #endif
      {
        const GLint location = glGetUniformLocation(shd_id, curr_uni_name);
        const GLint v0 = sampler_count;

        ++sampler_count;

        glUniform1i(location, v0);

        #ifdef OP_BUFFER_LOG_INFO
        context->log("Shader Sampler: %s at %d with slot %d", curr_uni_name, location, v0);
        #endif

        uniform_desc *uni(
          &context->uniform_descs[context->uniform_desc_size++]
        );

        uni->index    = v0;
        uni->type     = gl_type;
        uni->count    = 0;
        uni->program  = shd_id;

        strlcpy(
          uni->name,
          curr_uni_name,
          sizeof(char) * max_uniform_name_length()
        );

        // -- Extra Check -- //
        #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
        const GLenum err_code = glGetError();

        if(err_code)
        {
          context->log(
            "GL Error - Getting Sampler Uniform  %d : %s",
            err_code,
            get_error_msg(err_code)
          );
        }
        #endif
      }
      else
      {
        constexpr char gl_prefix[] = "gl_";

        if(strcmp(static_cast<char*>(curr_uni_name), gl_prefix) != 0)
        {
          ++uniform_data_count;

          const GLint index = glGetUniformLocation(shd_id, curr_uni_name);

          #ifdef OP_BUFFER_LOG_INFO
          context->log("Shader Uniform: %s", curr_uni_name);
          #endif

          uniform_desc *uni(
            &context->uniform_descs[context->uniform_desc_size++]
          );

          uni->program  = shd_id;
          uni->index    = index;
          uni->type     = gl_type;
          uni->count    = static_cast<int32_t>(size);

          strlcpy(
            uni->name,
            curr_uni_name,
            sizeof(char) * max_uniform_name_length()
          );
        }

        // -- Extra Check -- //
        #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
        const GLenum err_code = glGetError();

        if(err_code)
        {
          context->log(
            "GL Error - Getting Data Uniform  %d : %s",
            err_code,
            get_error_msg(err_code)
          );
        }
        #endif
      }
    }
  }

  if(compile_link_success)
  {
    // -- Update internal desc -- //
    internal_desc->program = shd_id;
    internal_desc->vs      = vs_id;
    internal_desc->gs      = gs_id;
    internal_desc->ps      = fs_id;

    // -- Extra Logging -- //
    #ifdef OP_BUFFER_LOG_INFO
    context->log("[");
    context->log("  Shader Created");
    context->log("  --");
    context->log("  Name: %s", cmd->name);
    context->log("  Uniform Count: %d", uniform_data_count);
    context->log("  Sampler Count: %d", sampler_count);
    context->log("  Program GL ID: %d", shd_id);
    context->log("  Vertex GL ID: %d", vs_id);
    context->log("  Geometry GL ID: %d", gs_id);
    context->log("  Fragment GL ID: %d", fs_id);
    context->log("]");
    #endif
  }
  #ifdef OP_BUFFER_LOG_INFO
  else
  {
    context->log("OP Error - Shader failed to be created");
  }
  #endif

  // -- Update in_out desc -- //
  cmd->desc->status = compile_link_success ? opStatus_VALID : opStatus_INVALID;
}


void
shader_destroy(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //
  command::cmd_shader_destroy *cmd = reinterpret_cast<command::cmd_shader_destroy*>(data);
  ogl::shader_desc *desc = &context->shader_descs[cmd->shader_id];
  assert(desc);

  // -- Extra Logging --//
  #ifdef OP_BUFFER_API_EXTRA_LOGGING
  context->log("Destroying Shader\n");
  #endif

  // -- Destroy Shaders -- //
  if(desc->program)   { glDeleteProgram(desc->program); }
  if(desc->vs)        { glDeleteShader(desc->vs);       }
  if(desc->gs)        { glDeleteShader(desc->gs);       }
  if(desc->ps)        { glDeleteShader(desc->ps);       }

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code = glGetError();

  if(err_code)
  {
    context->log(
      "Destroying shader error %d : %s",
      err_code,
      get_error_msg(err_code)
    );
  }
  #endif
}



void
shader_bind(context_data *context, void *data)
{
  // -- Param Check -- //
  assert(context);
  assert(data);

  // -- Get Data -- //
  command::cmd_shader_bind *cmd_info = reinterpret_cast<command::cmd_shader_bind*>(data);
  ogl::shader_desc *desc = &context->shader_descs[cmd_info->shader_id];
  assert(desc);

  // -- Set Last Shader Bind -- //
  context->shader_last_bound = cmd_info->shader_id;

  // -- Bind shader -- //
  glUseProgram(desc->program);

  // -- Extra Check -- //
  #ifdef OP_BUFFER_API_OGL_EXTRA_CHECKS
  const GLenum err_code = glGetError();

  if(err_code)
  {
    context->log(
      "Shader Bind Error %d : %s",
      err_code,
      get_error_msg(err_code)
    );
  }
  #endif
}


} // ns
} // ns

#endif // api guard
