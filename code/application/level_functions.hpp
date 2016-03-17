#ifndef LEVEL_FUNCTIONS_INCLUDED_462EB3DA_B67B_466A_B924_38039858B785
#define LEVEL_FUNCTIONS_INCLUDED_462EB3DA_B67B_466A_B924_38039858B785


#include <math/vec/vec2.hpp>


namespace Level {


math::vec2
get_point_on_cirlce(const float a);


constexpr float
get_radius() { return 5.f; }


constexpr float
get_bottom_of_level() { return -30.f; }


constexpr float
get_far_death_zone() { return get_bottom_of_level() * 2; }



constexpr float
get_top_of_level() { return 0.f; }


} // ns


#endif // inc guard