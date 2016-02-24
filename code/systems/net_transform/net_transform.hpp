#ifndef NET_TRANSFORM_INCLUDED_
#define NET_TRANSFORM_INCLUDED_


#include "net_transform_structure.hpp"
#include <math/math.hpp>
#include <cstddef>


namespace Net_transform {


void
generate_baseline_transforms(Net_transform::Baseline out_baseline[],
                             const std::uint32_t number_of_baselines,
                             const math::transform transforms[],
                             const std::uint32_t number_of_transforms);


} // ns


#endif // inc guard