#ifndef RESOURCE_DATA_INCLUDED_B0311127_B368_4A55_B58E_37C7198A76EA
#define RESOURCE_DATA_INCLUDED_B0311127_B368_4A55_B58E_37C7198A76EA


#include "audio_pool.hpp"
#include "shader_data.hpp"
#include "material_data.hpp"
#include "mesh_data.hpp"
#include "texture_data.hpp"
#include "post_process_data.hpp"
#include "text_mesh_data.hpp"
#include "font_data.hpp"
#include "rasterized_glyphs_data.hpp"


namespace Resource_data {


struct Resources
{
  Shader_data   *shader_data     = nullptr;
  Material_data *material_data   = nullptr;
  Mesh_data     *mesh_data       = nullptr;
  Texture_data  *texture_data    = nullptr;
  Post_process_data *post_data   = nullptr;
  Text_mesh_data *text_mesh_data = nullptr;
  Audio_pool    *audio_pool      = nullptr;
  Font_data     *font_data       = nullptr;
  Rasterized_glyphs_data *glyphs_data = nullptr;
};


void
resources_init();


[[deprecated("Use shared ptr one")]]
Resources*
get_resources();


std::shared_ptr<Resource_data::Resources>
get_resource_data();


} // ns


#endif // inc guard