#ifndef RENDERER_DEBUG_TICK_INCLUDED_574F3755_B805_4AB4_B1C8_2FFDA9F66319
#define RENDERER_DEBUG_TICK_INCLUDED_574F3755_B805_4AB4_B1C8_2FFDA9F66319


#include <data/fwd_dec_collections.hpp>
#include <data/context_data.hpp>


namespace Engine {
namespace Renderer_debug_tick {


void
initialize();


void
think(std::shared_ptr<Data::World> world,
      std::shared_ptr<Data::Context> resources,
      const float dt,
      const float running_time,
      const uint32_t width,
      const uint32_t height);


void
de_initialize();


} // ns
} // ns


#endif // inc guard