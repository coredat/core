#ifndef ENGINE_INCLUDED_028C7BEE_E9AA_408E_AEA1_55EF0DDB370F
#define ENGINE_INCLUDED_028C7BEE_E9AA_408E_AEA1_55EF0DDB370F


#include <data/world_data/world_pools.hpp>
#include <data/global_data/resource_data.hpp>


namespace Engine {


void
initialize();


void
think(const std::shared_ptr<World_data::World> world_data, const float dt);


void
de_initialize();


} // ns


#endif // inc guard