#ifndef ENTITY_DATA_INCLUDED_3C4A4817_CDF4_4800_AA82_C5213542B9A7
#define ENTITY_DATA_INCLUDED_3C4A4817_CDF4_4800_AA82_C5213542B9A7


#include <transformations/entity/entity_common.hpp>
#include <data/fwd_dec_data.hpp>


namespace Entity_detail {


void
set_user_data(const util::generic_id this_id,
              Data::Entity_data *entity_data,
              const uintptr_t user_data);
  
  
uintptr_t
get_user_data(const util::generic_id this_id,
              Data::Entity_data *entity_data);


uint32_t
get_tags(const util::generic_id this_id,
         Data::Entity_data *entity_data);
  
  
bool
has_tag(const util::generic_id this_id,
        Data::Entity_data *entity_data,
        const uint32_t tag_id);
  
  
void
set_tags(const util::generic_id this_id,
         Data::Entity_data *entity_data,
         const uint32_t set_tags);
  
  
void
add_tag(const util::generic_id this_id,
        Data::Entity_data *entity_data,
        const uint32_t add_tag);
  
  
void
remove_tag(const util::generic_id this_id,
           Data::Entity_data *entity_data,
           const uint32_t tag);


void
set_name(const util::generic_id this_id,
         Data::Entity_data *entity_data,
         const char* set_name);
  
  
const char*
get_name(const util::generic_id this_id,
         Data::Entity_data *entity_data);


} // ns


#endif // inc guard