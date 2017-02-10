#include <core/resources/shader_utils.hpp>
#include <core/resources/shader.hpp>
#include <core/common/directory.hpp>


namespace Core {
namespace Shader_utils {


Core::Shader
get_fullbright_shader()
{
  static Core::Shader fullbright;
  
  if(!fullbright)
  {
    char path[2048];
    memset(path, 0, sizeof(path));
    Directory::resource_path(path);
    
    strcat(path, "assets/shaders/core_fullbright.ogl");
    
    Core::Shader load_shader(path);
    
    if(load_shader)
    {
      fullbright = load_shader;
    }
  }
  
  return fullbright;
}


} // ns
} // ns