#ifndef TRANSFORM_INCLUDED_C656B188_F43B_4A0B_BC6C_7E26DA5E55B4
#define TRANSFORM_INCLUDED_C656B188_F43B_4A0B_BC6C_7E26DA5E55B4


#include <math/math.hpp>


namespace Transform {

/*!
  Gets the world up vector.
*/
inline math::vec3 world_up() { return math::vec3_init(0, +1, 0); }

/*!
  Gets the world left vector.
*/
inline math::vec3 world_left() { return math::vec3_init(+1, 0, 0); }

/*!
  Gets the world forward vector.
*/
inline math::vec3 world_forward() { return math::vec3_init(0, 0, -1); }



/*!
  Computes the wvp mats.
  \param input_transforms[] Is an array of transforms to be computed.
  \param number_of_input_transforms How many transforms are contained in the array.
  \param proj_view_mat The Projection View matrix.
  \param output_mats Where the results will go (must be first element of the output).
  \param number_of_output_mats How many wvp matrices will fit into this array.
  \param stride The stride of the output container (defaults to size of a mat4).
*/
void
transforms_to_wvp_mats(const math::transform input_transforms[],
                       const std::size_t number_of_input_transforms,
                       const math::mat4 view_proj_mat,
                       void *output_mats,
                       const std::size_t number_of_output_mats,
                       const std::size_t stride = sizeof(float) * 16);


/*!
  Computes the world mats.
  \param input_transforms[] Is an array of transforms to be computed.
  \param number_of_input_transforms How many transforms are contained in the array.
  \param output_mats Where the results will go (must be first element of the output).
  \param number_of_output_mats How many wvp matrices will fit into this array.
  \param stride The stride of the output container (defaults to size of a mat4).
*/
void
transforms_to_world_mats(const math::transform input_transforms[],
                         const std::size_t number_of_input_transforms,
                         void *output_mats,
                         const std::size_t number_of_output_mats,
                         const std::size_t stride = sizeof(float) * 16);

/*!
  Computes the forward direction.
  \param transform The currect transform.
  \param out_vec3 the resulting fwd vector.
*/
void
get_fwd_vec(const math::transform *transform, math::vec3 *out_vec3);


/*!
  Computes the left direction.
  \param transform The currect transform.
  \param out_vec3 the resulting left vector.
*/
void
get_left_vec(const math::transform *transform, math::vec3 *out_vec3);


/*!
  Computes the up direction.
  \param transform The currect transform.
  \param out_vec3 the resulting up vector.
*/
void
get_up_vec(const math::transform *transform, math::vec3 *out_vec3);


} // ns


#endif // inc guard