#include "ogl_shader_uniform.hpp"
#include "ogl_shader.hpp"
#include <assert.h>


namespace Ogl {


void
shader_uniforms_retrive(Shader_uniforms *out_uniforms, const Shader *shader)
{
  if(!shader)
  {
    assert(false);
    return;
  }

  glUseProgram(shader->program_id);

  // Get uniforms.
  GLint uniform_count, uniform_length;
  glGetProgramiv(shader->program_id, GL_ACTIVE_UNIFORMS, &uniform_count);
  glGetProgramiv(shader->program_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_length);

  std::vector<Sampler>samplers;
  samplers.reserve(uniform_count);
  
  std::vector<Uniform>constants;
  constants.reserve(uniform_count);

  for(GLint i = 0; i < uniform_count; ++i)
  {
    GLenum gl_type = 0;
    GLint length   = 0;
    GLint size     = 0;

    std::vector<GLchar> uni_name;
    uni_name.reserve(uniform_length);

    glGetActiveUniform(shader->program_id, i, uniform_length, &length, &size, &gl_type, uni_name.data());
    const std::string uniform_name(uni_name.data());

    // Is sampler?
    if((gl_type >= GL_SAMPLER_1D) && (gl_type <= GL_SAMPLER_2D_RECT_SHADOW))
    {
      const GLint location = glGetUniformLocation(shader->program_id, uniform_name.c_str());

      glUniform1i(location, static_cast<GLint>(samplers.size()));

      samplers.emplace_back(Sampler{uniform_name, static_cast<GLint>(samplers.size())});
    }
    // Then uniform
    else
    {
      const std::string prefix = "gl_";

      if(uniform_name.compare(0, prefix.length(), prefix) != 0)
      {
        const GLint index = glGetUniformLocation(shader->program_id, uniform_name.c_str());

        constants.emplace_back(Uniform{uniform_name, index, gl_type, static_cast<int32_t>(size)});
      }
    }
  }
  
  glUseProgram(0);

  samplers.shrink_to_fit();
  constants.shrink_to_fit();
  
  out_uniforms->samplers = std::move(samplers);
  out_uniforms->uniforms = std::move(constants);
}


bool
shader_uniforms_get_uniform_index(GLint *out_index, const Shader_uniforms *unis, const std::string &name)
{
  assert(out_index && unis);
  
  for(std::size_t i = 0; i < unis->uniforms.size(); ++i)
  {
    if(unis->uniforms[i].name == name)
    {
      (*out_index) = unis->uniforms[i].index;
      return true;
    }
  }
  
  return false;
}


bool
shader_uniforms_get_sampler_index(GLint *out_index, const Shader_uniforms *unis, const std::string &name)
{
  assert(out_index && unis);
  
  for(std::size_t i = 0; i < unis->uniforms.size(); ++i)
  {
    if(unis->samplers[i].name == name)
    {
      (*out_index) = unis->samplers[i].index;
      return true;
    }
  }

  return false;
}


} // ns