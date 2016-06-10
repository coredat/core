#ifndef RESOURCE_DATA_INCLUDED_B0311127_B368_4A55_B58E_37C7198A76EA
#define RESOURCE_DATA_INCLUDED_B0311127_B368_4A55_B58E_37C7198A76EA


#include "audio_pool.hpp"
#include "shader_data.hpp"
#include "material_data.hpp"
#include "mesh_data.hpp"
#include "texture_data.hpp"


namespace Resource_data {


struct Resources
{
  Shader_data   *shader_data    = nullptr;
  Material_data *material_data  = nullptr;
  Mesh_data     *mesh_data      = nullptr;
  Texture_data  *texture_data   = nullptr;

  Audio_pool    *audio_pool     = nullptr;
  
};


void
resources_init();


Resources*
get_resources();


} // ns


#endif // inc guard