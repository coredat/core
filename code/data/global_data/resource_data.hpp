#ifndef RESOURCE_DATA_INCLUDED_B0311127_B368_4A55_B58E_37C7198A76EA
#define RESOURCE_DATA_INCLUDED_B0311127_B368_4A55_B58E_37C7198A76EA


#include <data/fwd_dec_data.hpp>
#include "audio_pool.hpp"
#include <memory>


namespace Resource_data {


struct Resources
{
  Data::Shader_data       *shader_data     = nullptr;
  Data::Material_data     *material_data   = nullptr;
  Data::Mesh_data         *mesh_data       = nullptr;
  Data::Texture_data      *texture_data    = nullptr;
  Data::Post_process_data *post_data       = nullptr;
  Data::Text_mesh_data    *text_mesh_data  = nullptr;
  Audio_pool              *audio_pool      = nullptr;
  Data::Font_data         *font_data       = nullptr;
  Data::Font_glyph_data   *glyphs_data     = nullptr;
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