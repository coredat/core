#ifndef SCENE_GRAPH_TICK_INCLUDE_635CF1F4_1A74_4BD7_AC81_77677C8F198F
#define SCENE_GRAPH_TICK_INCLUDE_635CF1F4_1A74_4BD7_AC81_77677C8F198F


#include "../tick_information.hpp"
#include <data/world_data.hpp>
#include <memory>


namespace Engine {
namespace Scene_graph_tick {


void
initialize();


void
think(std::shared_ptr<Data::World> world, Tick_information *tick_info = nullptr);


void
de_initialize();


} // ns
} // ns


#endif // inc guard