#ifndef MATERIAL_DATA_INCLUDED_7D255780_AFCB_4CE4_A8E4_251B78F0729B
#define MATERIAL_DATA_INCLUDED_7D255780_AFCB_4CE4_A8E4_251B78F0729B


#include <utilities/generic_id.hpp>
#include <stdint.h>


namespace Resource_data {


struct Material
{
  util::generic_id    shader_id;
  util::generic_id    texture_map_01;
};


struct Material_data
{
  util::generic_id        *material_id    = nullptr;
  char                    *material_name  = nullptr;
  Material                *material       = nullptr;
  
  uint32_t                size            = 0;
  const uint32_t          capacity        = 0;
};


void
lock(Material_data *data);


void
unlock(Material_data *data);


void
material_data_init(Material_data *data, const uint32_t size_hint);


void
material_data_free(Material_data *data);


util::generic_id
material_data_add(Material_data *data,
                  const char *name,
                  const util::generic_id shader_id,
                  const util::generic_id texture_map_01);


const char *
material_data_get_name(Material_data *data,
                       const util::generic_id id);


bool
material_data_exsits(const Material_data *data,
                     const util::generic_id id,
                     size_t *out_index);
  

} // ns


#endif // inc guard


