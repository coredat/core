#ifndef WORLD_INDEX_INCLUDED_03550D3D_E675_4598_B9E2_82B4F62C4FCC
#define WORLD_INDEX_INCLUDED_03550D3D_E675_4598_B9E2_82B4F62C4FCC


#include <core/common/core_types.hpp>
#include <data/world_data/world_data.hpp>


/*
  To decouple the world data from the Core::World we store
  all active worlds here.
*/
namespace Core_detail {


uint32_t
world_index_add_world_data(const uint32_t entity_size_hint);


std::shared_ptr<World_data::World>
world_index_get_world_data(const uint32_t id);


void
world_index_release_world_data(const uint32_t id);


} // ns


#endif // inc guard