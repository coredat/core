#include <core/resources/texture_utils.hpp>
#include <core/resources/texture.hpp>
#include <core/common/directory.hpp>


namespace Core {
namespace Texture_utils {


Core::Texture
get_no_texture()
{
  static Core::Texture no_texture;
  
  if(!no_texture)
  {
    char path[2048];
    memset(path, 0, sizeof(path));
    Directory::resource_path(path);
    
    strcat(path, "assets/textures/core_no_texture.png");
    
    Core::Texture load_texture(path);
    
    if(load_texture)
    {
      no_texture = load_texture;
    }
  }
  
  return no_texture;
}


} // ns
} // ns
