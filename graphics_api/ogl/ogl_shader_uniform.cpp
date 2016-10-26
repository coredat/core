#include "ogl_shader_uniform.hpp"
#include "ogl_shader.hpp"
#include <utilities/optimizations.hpp>


namespace Ogl {


void
shader_uniforms_retrive(Shader_uniforms *out_uniforms,
                        const Shader *shader)
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
  glGetProgramiv(shader->program_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &uniform_length);

  // Allocate space for uniforms.
  Uniform *unis = new Uniform[uniform_count];
  
  // Allocate space for array of names.
  const uint32_t number_of_chars = (uniform_count * uniform_length) + uniform_count;
  char *names = new char[number_of_chars];
  memset(names, 0, sizeof(char) * number_of_chars);
  
  // Counters
  uint32_t  name_count         = 0;
  uint32_t  uni_count          = 0;
  GLint     number_of_samplers = 0;
  
  out_uniforms->uniform_count = uniform_count;

  GLchar curr_uni_name[512];

  for(GLint i = 0; i < uniform_count; ++i)
  {
    // Reset the buffer.
    memset(curr_uni_name, 0, sizeof(curr_uni_name));
  
    GLenum gl_type = 0;
    GLint length   = 0;
    GLint size     = 0;

    glGetActiveUniform(shader->program_id,
                       i,
                       uniform_length,
                       &length,
                       &size,
                       &gl_type,
                       curr_uni_name);
    
    strcat(&names[name_count], curr_uni_name);
    name_count += strlen(&names[name_count]);
    name_count += 1; // We want to skip over the null terminator
    
    // Make sure we are not overflowing.
    assert(name_count < number_of_chars);

    // Is sampler?
    if((gl_type >= GL_SAMPLER_1D) && (gl_type <= GL_SAMPLER_2D_RECT_SHADOW))
    {
      const GLint location = glGetUniformLocation(shader->program_id, curr_uni_name);

      glUniform1i(location, number_of_samplers);
      
      unis[uni_count++] = Uniform{number_of_samplers, gl_type, 0};
      
      ++number_of_samplers;
    }
    else
    {
      constexpr char gl_prefix[] = "gl_";

      if(strcmp(static_cast<char*>(curr_uni_name), gl_prefix) != 0)
      {
        const GLint index = glGetUniformLocation(shader->program_id, curr_uni_name);
        
        unis[uni_count++] = Uniform{index, gl_type, static_cast<int32_t>(size)};
      }
    }
  }
  
  // Assign the data to the object.
  out_uniforms->uniform_count    = uniform_count;
  out_uniforms->uniform_arr      = unis;
  out_uniforms->uniform_name_arr = names;
  
  glUseProgram(0);
}


bool
shader_uniforms_get_uniform_index(Uniform *out_index, const Shader_uniforms *unis, const char *name)
{
  assert(out_index && unis);
  
  // Search names
  uint32_t index = 0;
  uint32_t name_offset = 0;
  
  for(uint32_t i = 0; i < unis->uniform_count; ++i)
  {
    if(strcmp(name, &unis->uniform_name_arr[name_offset]) == 0)
    {
      (*out_index) = unis->uniform_arr[i];
      return true;
    }
    
    ++index;
    name_offset += strlen(&unis->uniform_name_arr[name_offset]) + 1;
  }
  
  // Make it invalid
  out_index->index = -1;
  out_index->number_of_elements = 0;
  out_index->type = 0;
  
  return false;
}


void
shader_uniforms_apply(const Uniform uniform_to_apply, void *data)
{
  switch(uniform_to_apply.type)
  {
    case(GL_FLOAT):       glUniform1fv(uniform_to_apply.index, uniform_to_apply.number_of_elements, (GLfloat*)data);  return;
    case(GL_FLOAT_VEC2):  glUniform2fv(uniform_to_apply.index, uniform_to_apply.number_of_elements, (GLfloat*)data);  return;
    case(GL_FLOAT_VEC3):  glUniform3fv(uniform_to_apply.index, uniform_to_apply.number_of_elements, (GLfloat*)data);  return;
    case(GL_FLOAT_VEC4):  glUniform4fv(uniform_to_apply.index, uniform_to_apply.number_of_elements, (GLfloat*)data);  return;
    case(GL_INT):         glUniform1iv(uniform_to_apply.index, uniform_to_apply.number_of_elements, (GLint*)data);    return;
    case(GL_INT_VEC2):    glUniform2iv(uniform_to_apply.index, uniform_to_apply.number_of_elements, (GLint*)data);    return;
    case(GL_INT_VEC3):    glUniform3iv(uniform_to_apply.index, uniform_to_apply.number_of_elements, (GLint*)data);    return;
    case(GL_INT_VEC4):    glUniform4iv(uniform_to_apply.index, uniform_to_apply.number_of_elements, (GLint*)data);    return;
    case(GL_BOOL):        glUniform1iv(uniform_to_apply.index, uniform_to_apply.number_of_elements, (GLint*)data);    return;
    case(GL_BOOL_VEC2):   glUniform2iv(uniform_to_apply.index, uniform_to_apply.number_of_elements, (GLint*)data);    return;
    case(GL_BOOL_VEC3):   glUniform3iv(uniform_to_apply.index, uniform_to_apply.number_of_elements, (GLint*)data);    return;
    case(GL_BOOL_VEC4):   glUniform4iv(uniform_to_apply.index, uniform_to_apply.number_of_elements, (GLint*)data);    return;
    case(GL_FLOAT_MAT2):  glUniformMatrix2fv(uniform_to_apply.index, uniform_to_apply.number_of_elements, GL_FALSE, (GLfloat*)data); return;
    case(GL_FLOAT_MAT3):  glUniformMatrix3fv(uniform_to_apply.index, uniform_to_apply.number_of_elements, GL_FALSE, (GLfloat*)data); return;
    case(GL_FLOAT_MAT4):  glUniformMatrix4fv(uniform_to_apply.index, uniform_to_apply.number_of_elements, GL_FALSE, (GLfloat*)data); return;
    case(GL_SAMPLER_1D):
    case(GL_SAMPLER_1D_ARRAY):
    case(GL_SAMPLER_1D_ARRAY_SHADOW):
    case(GL_SAMPLER_1D_SHADOW):
    case(GL_SAMPLER_2D):
    case(GL_SAMPLER_2D_ARRAY):
    case(GL_SAMPLER_2D_ARRAY_SHADOW):
    case(GL_SAMPLER_2D_MULTISAMPLE):
    case(GL_SAMPLER_2D_MULTISAMPLE_ARRAY):
    case(GL_SAMPLER_2D_RECT):
    case(GL_SAMPLER_2D_RECT_SHADOW):
    case(GL_SAMPLER_2D_SHADOW):
    case(GL_SAMPLER_3D):
    {
      // There are other samplers but do we need them?
      GLint tex_id = *static_cast<GLint*>(data);
      
      glActiveTexture(GL_TEXTURE0 + uniform_to_apply.index);
      glBindTexture(GL_TEXTURE_2D, tex_id);
      return;
    }

    default:
      return;
  };

  UNREACHABLE;
}


} // ns