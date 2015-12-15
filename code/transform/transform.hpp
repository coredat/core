#ifndef TRANSFORM_INCLUDED_
#define TRANSFORM_INCLUDED_


#include <math/math.hpp>


namespace Transform {


/*!
  Computes wvp mats.
*/
void transforms_to_wvp(const math::transform input_transforms[],
                       const std::size_t number_of_input_transforms,
                       const math::mat4 proj_mat,
                       const math::mat4 view_mat,
                       void *output_mats,
                       const std::size_t number_of_output_mats,
                       const std::size_t stride = 0);


} // ns


#endif // inc guard