#include "ogl_shader.hpp"
#include <utilities/logging.hpp>
#include <assert.h>
#include <cstring>


namespace Ogl {


void
shader_create(Shader *out_shader,
                     const char *vert_shader_code,
                     const char *geo_shader_code,
                     const char *frag_shader_code)
{
  assert(out_shader);
  
  auto geo_exists = [](const char *geo_shader_code)
  {
    return (geo_shader_code && strlen(geo_shader_code));
  };
  
  // Compile Vertex Shader
  const GLint vert_shader_id = Detail::compile_shader(GL_VERTEX_SHADER, vert_shader_code);

  #ifdef OGL_EXTRA_ERROR_CHECKS
  const GLenum vert_compile_err_code = glGetError();
  if(vert_compile_err_code != GL_NO_ERROR)
  {
    LOG_GL_ERROR(vert_compile_err_code, "Compiling vertex shader");
  }
  #endif
  
  // Compile Geo shader
  GLuint geo_shader_id(0);
  if(geo_exists(geo_shader_code))
  {
    geo_shader_id = Detail::compile_shader(GL_GEOMETRY_SHADER, geo_shader_code);

    #ifdef OGL_EXTRA_ERROR_CHECKS
    const GLenum geo_compile_err_code = glGetError();
    if(geo_compile_err_code != GL_NO_ERROR)
    {
      LOG_GL_ERROR(geo_compile_err_code, "Compiling geometry shader");
    }
    #endif
  }
  
  // Compile Fragment shader
  const auto frag_shader_id = Detail::compile_shader(GL_FRAGMENT_SHADER, frag_shader_code);
  
  #ifdef OGL_EXTRA_ERROR_CHECKS
  const GLenum frag_compile_err_code = glGetError();
  if(frag_compile_err_code != GL_NO_ERROR)
  {
    LOG_GL_ERROR(frag_compile_err_code, "Compiling fragment shader");
  }
  #endif
  
  const auto program_id = glCreateProgram();
  
  #ifdef OGL_EXTRA_ERROR_CHECKS
  const GLenum program_compile_err_code = glGetError();
  if(program_compile_err_code != GL_NO_ERROR)
  {
    LOG_GL_ERROR(program_compile_err_code, "Compiling program");
  }
  #endif
  
  glAttachShader(program_id, vert_shader_id);
  
  if(geo_exists(geo_shader_code))
  {
    glAttachShader(program_id, geo_shader_id);
  }
  
  glAttachShader(program_id, frag_shader_id);
  glLinkProgram(program_id);

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
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
    
    if(log_length)
    {
      GLchar output_log[512];
      memset(output_log, 0, sizeof(output_log));
      
      glGetProgramInfoLog(program_id, log_length, 0, output_log);
      
      GLint is_linked = false;
      glGetShaderiv(program_id, GL_LINK_STATUS, &is_linked);
      
      if(is_linked)
      {
        LOG_WARNING(output_log);
      }
      else
      {
        LOG_ERROR(output_log);
      }
    }
  }
  
  // Did it link
  GLint is_linked;
  glGetProgramiv(program_id, GL_LINK_STATUS, &is_linked);
  
  if(!is_linked) // Failed
  {
    LOG_ERROR("Failed to link shader program.")
    
    Shader clean_up;
    clean_up.program_id     = program_id;
    clean_up.vert_shader_id = vert_shader_id;
    clean_up.geo_shader_id  = geo_shader_id;
    clean_up.frag_shader_id = frag_shader_id;
    
    shader_destroy(&clean_up);
    
    return;
  }
  
  // Success
  out_shader->vert_shader_id  = vert_shader_id;
  out_shader->geo_shader_id   = geo_shader_id;
  out_shader->frag_shader_id  = frag_shader_id;
  out_shader->program_id      = program_id;
}


void
shader_destroy(Shader *shader)
{
  // Param check
  if(!shader)
  {
    assert(false);
    
    LOG_ERROR("'shader' is not a valid paramater in 'shader_destroy'");
    
    return;
  }

  if(shader->program_id)      { glDeleteProgram(shader->program_id);    }
  if(shader->vert_shader_id)  { glDeleteShader(shader->vert_shader_id); }
  if(shader->geo_shader_id)   { glDeleteShader(shader->geo_shader_id);  }
  if(shader->frag_shader_id)  { glDeleteShader(shader->frag_shader_id); }
}


void
shader_bind(const Shader *shader_to_bind)
{
  if(!shader_to_bind)
  {
    glUseProgram(0);
  }
  else
  {
    glUseProgram(shader_to_bind->program_id);
    
    #ifdef OGL_EXTRA_ERROR_CHECKS
    const GLenum err_code = glGetError();
    if(err_code != GL_NO_ERROR)
    {
      LOG_GL_ERROR(err_code, "Binding shader");
    }
    #endif
  }
}


namespace Detail {


GLuint
compile_shader(const GLenum shader_type,
               const char *shader_code)
{
  // Param check
  if(strlen(shader_code) == 0)
  {
    return 0;
  }

  const GLuint shader_id = glCreateShader(shader_type);
  
  // Compile the shader.
  {
    glShaderSource(shader_id, 1, &shader_code, NULL);
    glCompileShader(shader_id);
  }

  // Log
  {
    GLint log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
    
    if(log_length)
    {
      GLchar output_log[512];
      memset(output_log, 0, sizeof(output_log));
      
      glGetShaderInfoLog(shader_id, log_length, 0, output_log);
      
      // Did it compile
      GLint is_compiled = false;
      glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);

      if(is_compiled == GL_FALSE)
      {
        LOG_ERROR(output_log);
        return 0;
      }
      else
      {
        LOG_WARNING(output_log);
      }
    }
  }

  return shader_id;
}


} // ns


} // ns