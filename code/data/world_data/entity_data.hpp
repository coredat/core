#ifndef ENTITY_DATA_INCLUDED_18E6DF4A_1B5A_44CE_A75F_FBC3054389B2
#define ENTITY_DATA_INCLUDED_18E6DF4A_1B5A_44CE_A75F_FBC3054389B2


#include <utilities/generic_id.hpp>
#include <stdint.h>
#include <stddef.h>


namespace World_data {


struct Entity_data
{
  util::generic_id      *entity_id    = nullptr;
  char                  **entity_name = nullptr;
  uint32_t              *tags         = nullptr;
  
  char                  *entity_name_buffer = nullptr;

  uint32_t              size          = 0;
  const uint32_t        capacity      = 0;
};


void
lock(Entity_data *data);


void
unlock(Entity_data *data);


void
entity_data_init(Entity_data *data,
                 const uint32_t size_hint);


void
entity_data_free(Entity_data *data);


void
entity_data_add_entity(Entity_data &data,
                       const util::generic_id id);


void
entity_data_remove_entity(Entity_data &data,
                          const util::generic_id id);


bool
entity_data_exists(Entity_data *data,
                   const util::generic_id id);


} // ns


#endif // inc guard