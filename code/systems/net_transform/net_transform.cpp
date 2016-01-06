#include "net_transform.hpp"


namespace Net_transform {


void
generate_baseline_transforms(Net_transform::Baseline baseline[],
                             const std::size_t number_of_baselines,
                             const math::transform transforms[],
                             const std::size_t number_of_transforms)
{
  const std::size_t number_to_calculate = std::min(number_of_baselines, number_of_transforms);
  
  for(std::size_t i = 0; i < number_to_calculate; ++i)
  {
    const math::transform *curr_trans = &transforms[i];
    math::vec3_to_array(curr_trans->position, baseline[i].position);
    math::quat_to_array(curr_trans->rotation, baseline[i].rotation);
  }
}



} // ns