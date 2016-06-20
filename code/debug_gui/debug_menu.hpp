#ifndef DEBUG_GUI_INCLUDED_F55C49F8_D3FD_4F82_A875_A1313D1F54E3
#define DEBUG_GUI_INCLUDED_F55C49F8_D3FD_4F82_A875_A1313D1F54E3


#include <data/world_data/world.hpp>
#include <core/world/world_fwd.hpp>
#include <stdint.h>


namespace Debug_menu {


void
display_global_data_menu();


void
display_world_data_menu(World_data::World *world_data,
                        const float dt,
                        const float dt_mul,
                        const uint32_t draw_calls,
                        const uint32_t render_passes);


} // ns


#endif // inc guard