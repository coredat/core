#include <transformations/entity/entity_data.hpp>
#include <data/world_data/world_pools.hpp>
#include <data/world_data/entity_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace Entity_detail {


void
set_user_data(const util::generic_id this_id,
              World_data::World *world,
              const uintptr_t user_data)
{
  // Check is valid
  if(!is_valid(this_id, world, true)) {
    assert(false); return;
  }
  
  // Set data
  {
    auto entity_data = world->entity;
    assert(entity_data);
    
    World_data::data_lock(entity_data);
    
    if(!World_data::entity_data_set_property_user_data(entity_data, this_id, user_data))
    {
      assert(false);
      LOG_WARNING(Error_string::data_not_set());
    }
    
    World_data::data_unlock(entity_data);
  }
}


uintptr_t
get_user_data(const util::generic_id this_id,
              World_data::World *world)
{
  // Check valid
  if(!is_valid(this_id, world, true)) {
    assert(false); return 0;
  }
  
  // Return data
  uintptr_t user_data(0);
  
  // Get data
  {
    auto entity_data = world->entity;
    assert(entity_data);
    
    World_data::data_lock(entity_data);
    
    if(!World_data::entity_data_get_property_user_data(entity_data, this_id, &user_data))
    {
      assert(false);
      LOG_WARNING(Error_string::data_not_found());
    }
    
    World_data::data_unlock(entity_data);
  }
  
  return user_data;
}


uint32_t
get_tags(const util::generic_id this_id,
         World_data::World *world)
{
  // Check valid
  if(!is_valid(this_id, world, true)) {
    assert(false); return 0;
  }

  // Return data
  uint32_t tags = 0;
  
  // Get data
  {
    auto entity_data = world->entity;
    assert(false);
    
    World_data::data_lock(entity_data);
    assert(entity_data);
    
    if(!World_data::entity_data_get_property_tag(entity_data, this_id, &tags))
    {
      assert(false);
      LOG_WARNING(Error_string::data_not_found());
    }
    
    World_data::data_unlock(entity_data);
  }
  
  return tags;
}


bool
has_tag(const util::generic_id this_id,
        World_data::World *world,
        const uint32_t tag_id)
{
  // Check valid
  if(!is_valid(this_id, world, true)) {
    assert(false); return 0;
  }
  
  const uint32_t tags = get_tags(this_id, world);
  
  return !!(tags & tag_id);
}


void
set_tags(const util::generic_id this_id,
         World_data::World *world,
         const uint32_t set_tags)
{
  // Check valid
  if(!is_valid(this_id, world, true)) {
    assert(false); return;
  }
  
  // Set data
  {
    auto entity_data = world->entity;
    assert(entity_data);
    
    World_data::data_lock(entity_data);
    
    if(!World_data::entity_data_set_property_tag(entity_data, this_id, set_tags))
    {
      assert(false);
      LOG_WARNING(Error_string::data_not_found());
    }
    
    data_unlock(entity_data);
  }
}


void
add_tag(const util::generic_id this_id,
        World_data::World *world,
        const uint32_t add_tag)
{
  // Check valid
  if(!is_valid(this_id, world, true)) {
    assert(false); return;
  }

  const uint32_t tags = get_tags(this_id, world);
  set_tags(this_id, world, tags | add_tag);
}


void
remove_tag(const util::generic_id this_id,
           World_data::World *world,
           const uint32_t tag)
{
  // Check is valid
  if(!is_valid(this_id, world, true)) {
    assert(false); return;
  }

  // Remove tag
  const uint32_t tags = get_tags(this_id, world);
  set_tags(this_id, world, tags &~ tag);
}


void
set_name(const util::generic_id this_id,
         World_data::World *world,
         const char* set_name)
{
  // Check is valid
  if(!is_valid(this_id, world, true)) {
    assert(false); return;
  }
  
  // Set data
  {
    auto entity_data = world->entity;
    assert(entity_data);

    World_data::data_lock(world->entity);
    
    if(!World_data::entity_data_set_property_name(world->entity, this_id, set_name))
    {
      assert(false);
      LOG_WARNING(Error_string::data_not_set());
    }
    
    World_data::data_unlock(world->entity);
  }
}
  
  
const char*
get_name(const util::generic_id this_id,
         World_data::World *world)
{
  // Check is valid
  if(!is_valid(this_id, world, true)) {
    assert(false); return nullptr;
  }
  
  // Return data
  const char *name = "";
  
  // Get data
  {
    auto entity_data = world->entity;
    assert(entity_data);
    
    World_data::data_lock(entity_data);
    
    if(!World_data::entity_data_get_property_name(world->entity, this_id, &name))
    {
      assert(false);
      LOG_WARNING(Error_string::data_not_found());
    }
    
    World_data::data_unlock(entity_data);
  }
  
  return name;
}


} // ns