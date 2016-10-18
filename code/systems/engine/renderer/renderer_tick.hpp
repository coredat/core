#ifndef RENDERER_TICK_INCLUDED_79D17AAA_89E5_46D1_A2CB_A854549E7C03
#define RENDERER_TICK_INCLUDED_79D17AAA_89E5_46D1_A2CB_A854549E7C03


#include "../tick_information.hpp"
#include <data/fwd_dec_collections.hpp>
#include <data/context_data.hpp>


namespace Engine {
namespace Renderer_tick {


void
initialize();


void
think(std::shared_ptr<Data::World> world,
      std::shared_ptr<Data::Context> resources,
      const float dt,
      const float running_time,
      const uint32_t width,
      const uint32_t height,
      Tick_information *out_tick_info = nullptr);


void
de_initialize();


} // ns
} // ns


#endif // inc guard