#include "ogl_shader.hpp"
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>


namespace Ogl {


void
shader_create(Shader *out_shader,
                     const char *vert_shader_code,
                     const char *geo_shader_code,
                     const char *frag_shader_code,
                     std::ostream *log)
{
  assert(out_shader);
  
  auto create_shader = [](const GLenum shader_type, const char *shader_code, std::ostream *log) -> GLuint
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
    std::vector<GLchar> output_log;
    {
      GLint log_length = 0;
      glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

      output_log.resize(log_length);
      glGetShaderInfoLog(shader_id, log_length, 0, output_log.data());
    }

		if(!output_log.empty() && log)
		{
      std::string log_str;
      log_str.reserve(output_log.size());
      log_str.append(output_log.data());

		  // Did it compile
		  GLint is_compiled = false;
		  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);

      if(is_compiled == GL_FALSE)
      {
        (*log) << "Ogl::shader_create - errors compiling shader.\n";
        (*log) << log_str << "\n";
        return 0;
      }
      else
      {
        (*log) << "Ogl::shader_create - warnings compiling shader.\n";
        (*log) << log_str << "\n";
      }
		}

    return shader_id;
  };
  
  const auto vert_shader_id = create_shader(GL_VERTEX_SHADER,   vert_shader_code, log);
  Ogl::error_check("creating vert shader", log);
  
  GLuint geo_shader_id(0);
  if(strlen(geo_shader_code))
  {
    geo_shader_id = create_shader(GL_GEOMETRY_SHADER, geo_shader_code,  log);
    Ogl::error_check("creating geo shader", log);
  }
  
  const auto frag_shader_id = create_shader(GL_FRAGMENT_SHADER, frag_shader_code, log);
  Ogl::error_check("creating frag shader", log);
  
  const auto program_id = glCreateProgram();
  Ogl::error_check("creating program", log);
  
  glAttachShader(program_id, vert_shader_id);
  Ogl::error_check("attach shader to program", log);
  
  if(strlen(geo_shader_code))
  {
    glAttachShader(program_id, geo_shader_id);
    Ogl::error_check("attach shader to program", log);
  }
  
  glAttachShader(program_id, frag_shader_id);
  glLinkProgram(program_id);
  Ogl::error_check("linking program", log);
 
  // Log
  std::vector<GLchar> output_log;
  {
    GLint log_length;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

		output_log.resize(log_length);
  }
  
  if(!output_log.empty() && log)
  {
    std::string log_str;
    log_str.reserve(output_log.size());
    log_str.append(output_log.data());
    
    if(!log_str.empty())
    {
      (*log) << "Ogl::shader_create - issues linking shader.\n";
      (*log) << log_str << "\n";
    }
  }
  
  // Did it link
  GLint is_linked;
  glGetProgramiv(program_id, GL_LINK_STATUS, &is_linked);
  
  if(!is_linked) // Failed
  {
    if(log)
    {
      (*log) << "Ogl::shader_create - failed to link shader program.\n";
    }
    
    Shader clean_up;
    clean_up.program_id     = program_id;
    clean_up.vert_shader_id = vert_shader_id;
    clean_up.geo_shader_id  = geo_shader_id;
    clean_up.frag_shader_id = frag_shader_id;
    
    shader_destroy(&clean_up, log);
    
    return;
  }
  
  // Success
  out_shader->vert_shader_id  = vert_shader_id;
  out_shader->geo_shader_id   = geo_shader_id;
  out_shader->frag_shader_id  = frag_shader_id;
  out_shader->program_id      = program_id;
}


void
shader_destroy(Shader *shader, std::ostream *log)
{
  // Param check
  if(!shader)
  {
    assert(false);
    
    if(log)
    {
      (*log) << "'shader' is not a valid paramater in 'shader_destroy'";
    }
    
    return;
  }

  if(shader->program_id)      { glDeleteProgram(shader->program_id);    }
  if(shader->vert_shader_id)  { glDeleteShader(shader->vert_shader_id); }
  if(shader->geo_shader_id)   { glDeleteShader(shader->geo_shader_id);  }
  if(shader->frag_shader_id)  { glDeleteShader(shader->frag_shader_id); }
}


void
shader_bind(Shader *shader_to_bind)
{
  if(!shader_to_bind)
  {
    glUseProgram(0);
  }
  else
  {
    glUseProgram(shader_to_bind->program_id);
    Ogl::error_check("Binding shader", &std::cout);
  }
}


} // ns