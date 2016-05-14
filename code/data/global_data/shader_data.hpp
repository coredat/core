#ifndef SAHDER_DATA_INCLUDED_B7E05816_B063_4C7F_9363_2B8932AD5590
#define SAHDER_DATA_INCLUDED_B7E05816_B063_4C7F_9363_2B8932AD5590


#include <graphics_api/ogl/ogl_shader.hpp>
#include <utilities/generic_id.hpp>


namespace Resource_data {


struct Shader_data
{
  util::generic_id    *shader_ids   = nullptr;
  char                *shader_name  = nullptr;
  Ogl::Shader         *shader       = nullptr;
  
  uint32_t            size          = 0;
  const uint32_t      capacity      = 0;
};


void
shader_data_init(Shader_data *data);


util::generic_id
shader_data_add_from_file(Shader_data *data,
                          const char *filename);


util::generic_id
shader_data_add_from_code(Shader_data *data,
                          const char *name,
                          const char *vs,
                          const char *gs,
                          const char *ps);


} // ns


#endif // inc guard