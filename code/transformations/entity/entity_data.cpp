#include <transformations/entity/entity_data.hpp>
#include <data/world/entity_data.hpp>
#include <assert.h>


namespace Entity_detail {


void
set_user_data(const util::generic_id this_id,
              Data::Entity_data *entity_data,
              const uintptr_t user_data)
{
  // Check is valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }
  
  // Set data
  {
    Data::data_lock(entity_data);
    Data::entity_set_user_data(entity_data, this_id, &user_data);
    Data::data_unlock(entity_data);
  }
}


uintptr_t
get_user_data(const util::generic_id this_id,
              Data::Entity_data *entity_data)
{
  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return 0;
  }
  
  // Return data
  uintptr_t user_data(0);
  
  // Get data
  {
    Data::data_lock(entity_data);
    Data::entity_get_user_data(entity_data, this_id, &user_data);
    Data::data_unlock(entity_data);
  }
  
  return user_data;
}


uint32_t
get_tags(const util::generic_id this_id,
         Data::Entity_data *entity_data)
{
  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return 0;
  }

  // Return data
  uint32_t tags = 0;
  
  // Get data
  {
    Data::data_lock(entity_data);
    Data::entity_get_tags(entity_data, this_id, &tags);
    Data::data_unlock(entity_data);
  }
  
  return tags;
}


bool
has_tag(const util::generic_id this_id,
        Data::Entity_data *entity_data,
        const uint32_t tag_id)
{
  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return 0;
  }
  
  return !!(get_tags(this_id, entity_data) & tag_id);
}


void
set_tags(const util::generic_id this_id,
         Data::Entity_data *entity_data,
         const uint32_t set_tags)
{
  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }
  
  // Set data
  {
    Data::data_lock(entity_data);
    Data::entity_set_tags(entity_data, this_id, &set_tags);
    Data::data_unlock(entity_data);
  }
}


void
add_tag(const util::generic_id this_id,
        Data::Entity_data *entity_data,
        const uint32_t add_tag)
{
  // Check valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }

  const uint32_t tags = get_tags(this_id, entity_data);
  set_tags(this_id, entity_data, tags | add_tag);
}


void
remove_tag(const util::generic_id this_id,
           Data::Entity_data *entity_data,
           const uint32_t tag)
{
  // Check is valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }

  // Remove tag
  const uint32_t tags = get_tags(this_id, entity_data);
  set_tags(this_id, entity_data, tags &~ tag);
}


void
set_name(const util::generic_id this_id,
         Data::Entity_data *entity_data,
         const char* set_name)
{
  // Check is valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }
  
  // Set data
  {
    Data::data_lock(entity_data);
    Data::entity_set_name(entity_data, this_id, set_name, strlen(set_name));
    Data::data_unlock(entity_data);
  }
}
  
  
const char*
get_name(const util::generic_id this_id,
         Data::Entity_data *entity_data)
{
  // Check is valid
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return nullptr;
  }
  
  // Return data
  const char *name = "";
  
  // Get data
  {
    Data::data_lock(entity_data);
    Data::entity_get_name(entity_data, this_id, &name);
    Data::data_unlock(entity_data);
  }
  
  return name;
}


} // ns