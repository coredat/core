#include <core/model/model_utils.hpp>
#include <core/model/model.hpp>
#include <core/common/directory.hpp>



namespace Core {
namespace Model_utils {


Core::Model
get_unit_plane_model()
{
  static Core::Model plane_model;
  
  if(!plane_model)
  {
    char path[2048];
    memset(path, 0, sizeof(path));
    Directory::resource_path(path);
    
    strcat(path, "assets/models/core_unit_plane.obj");
    
    Core::Model load_model(path);
    
    if(load_model)
    {
      plane_model = load_model;
    }

  }
  
  return plane_model;
}


Core::Model
get_unit_cube_model()
{
  static Core::Model cube_model;
  
  if(!cube_model)
  {
    char path[2048];
    memset(path, 0, sizeof(path));
    Directory::resource_path(path);
    
    strcat(path, "assets/models/core_unit_cube.obj");
    
    Core::Model load_model(path);
    
    if(load_model)
    {
      cube_model = load_model;
    }

  }
  
  return cube_model;
}


} // ns
} // ns
