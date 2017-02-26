#include <core/resources/shader_utils.hpp>
#include <core/resources/shader.hpp>
#include <core/common/directory.hpp>


namespace {


inline Core::Shader
load_shd(const char *filename)
{
  char path[2048];
  memset(path, 0, sizeof(path));
  Core::Directory::resource_path(path);
  
  strcat(path, filename);
  
  return Core::Shader(path);
}


} // anon ns


namespace Core {
namespace Shader_utils {


Core::Shader
get_fullbright_shader()
{
  static Core::Shader fullbright;
  
  if(!fullbright)
  {
    fullbright = load_shd("assets/shaders/core_mat_fullbright.ogl");
  }
  
  return fullbright;
}


Core::Shader
get_fixed_directional_light_shader()
{
  static Core::Shader dir_light;
  
  if(!dir_light)
  {
    dir_light = load_shd("assets/shaders/core_mat_fixed_dir_light.ogl");
  }
  
  return dir_light;
}


} // ns
} // ns
