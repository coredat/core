#ifndef STATS_OVERLAY_INCLUDED_6E77771E_D19D_40EE_8C17_1A60F7FC6306
#define STATS_OVERLAY_INCLUDED_6E77771E_D19D_40EE_8C17_1A60F7FC6306


#include <stdint.h>


namespace Debug_menu {


void
display_world_stats(const float dt,
                    const float dt_mul,
                    const uint32_t draw_calls,
                    const uint32_t render_passes);


} // ns


#endif // inc guard