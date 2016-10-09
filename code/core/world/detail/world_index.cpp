#include <core/world/detail/world_index.hpp>
#include <core/entity/detail/entity_id.hpp>
#include <utilities/logging.hpp>
#include <assert.h>


namespace {


constexpr uint32_t max_worlds = 1 << Core_detail::entity_detail_world_bits();

std::atomic<uint32_t>               world_instance_id(0);
std::shared_ptr<World_data::World>  worlds[max_worlds];
uint32_t                            world_ids[max_worlds] = {0};


} // anon ns


namespace Core_detail {


uint32_t
world_index_add_world_data(const uint32_t entity_size_hint)
{
  // Find space
  for(uint32_t i = 0; i < max_worlds; ++i)
  {
    if(world_ids[i] == 0)
    {
      world_instance_id++;
      world_ids[i] = world_instance_id;
      worlds[i] = std::make_shared<World_data::World>(entity_size_hint);
      return world_instance_id;
    }
  }
  
  return 0;
}


std::shared_ptr<World_data::World>
world_index_get_world_data(const uint32_t id)
{
  for(uint32_t i = 0; i < max_worlds; ++i)
  {
    if(world_ids[i] == id)
    {
      return worlds[i];
    }
  }
  
  LOG_ERROR("Can't find world");
  return nullptr;
}


void
world_index_release_world_data(const uint32_t id)
{
  for(uint32_t i = 0; i < max_worlds; ++i)
  {
    if(world_ids[i] == id)
    {
      world_ids[i] = 0;
      worlds[i].reset();
    }
  }
}


} // ns