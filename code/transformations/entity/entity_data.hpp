#ifndef ENTITY_DATA_INCLUDED_3C4A4817_CDF4_4800_AA82_C5213542B9A7
#define ENTITY_DATA_INCLUDED_3C4A4817_CDF4_4800_AA82_C5213542B9A7


#include <transformations/entity/entity_common.hpp>


namespace Entity_detail {


void
set_user_data(const util::generic_id this_id,
              World_data::World *world,
              const uintptr_t user_data);
  
  
uintptr_t
get_user_data(const util::generic_id this_id,
              World_data::World *world);


uint32_t
get_tags(const util::generic_id this_id,
         World_data::World *world);
  
  
bool
has_tag(const util::generic_id this_id,
        World_data::World *world,
        const uint32_t tag_id);
  
  
void
set_tags(const util::generic_id this_id,
         World_data::World *world,
         const uint32_t set_tags);
  
  
void
add_tag(const util::generic_id this_id,
        World_data::World *world,
        const uint32_t add_tag);
  
  
void
remove_tag(const util::generic_id this_id,
           World_data::World *world,
           const uint32_t tag);


void
set_name(const util::generic_id this_id,
         World_data::World *world,
         const char* set_name);
  
  
const char*
get_name(const util::generic_id this_id,
         World_data::World *world);


} // ns


#endif // inc guard