#include "transformations.hpp"


namespace Transform {


void
transforms_to_wvp_mats(const math::transform input_transforms[],
                       const std::uint32_t number_of_input_transforms,
                       const math::mat4 view_proj_mat,
                       void  *output_mats,
                       const std::uint32_t number_of_output_mats,
                       const std::uint32_t byte_stride)
                        {
                          const uint32_t number_to_process = static_cast<uint32_t>(math::min(number_of_input_transforms, number_of_output_mats));
                        
                          for(uint32_t i = 0; i < number_to_process; ++i)
                          {
                            // Calculate wvp matrix.
                            const math::mat4 world_mat = math::transform_get_world_matrix(input_transforms[i]);
                            const math::mat4 wvp_mat = math::mat4_multiply(world_mat, view_proj_mat);
                            
                            // Starting point taking into account stride.
                            uint8_t *data = &reinterpret_cast<uint8_t*>(output_mats)[byte_stride * i];
                            
                            // Insert point after stride taken into account.
                            float *out_data = reinterpret_cast<float*>(data);
                            
                            // Insert result into output.
                            math::mat4_to_array(wvp_mat, out_data);
                          }
                        }


void
transforms_to_world_mats(const math::transform input_transforms[],
                         const uint32_t number_of_input_transforms,
                         void  *output_mats,
                         const uint32_t number_of_output_mats,
                         const uint32_t byte_stride)
                         {
                           const uint32_t number_to_process = static_cast<uint32_t>(math::min(number_of_input_transforms, number_of_output_mats));
                         
                           for(uint32_t i = 0; i < number_to_process; ++i)
                           {
                             // Calculate wvp matrix.
                             const math::mat4 world_mat = math::transform_get_world_matrix(input_transforms[i]);
                             
                             // Starting point taking into account stride.
                             uint8_t *data = &reinterpret_cast<uint8_t*>(output_mats)[byte_stride * i];
                             
                             // Insert point after stride taken into account.
                             float *out_data = reinterpret_cast<float*>(data);
                             
                             // Insert result into output.
                             math::mat4_to_array(world_mat, out_data);
                           }
                         }


void
get_fwd_vec(const math::transform *transform, math::vec3 *out_vec3)
{
  // Param check.
  assert(transform && out_vec3);
  
  *out_vec3 = math::quat_rotate_point(transform->rotation, world_forward());
}


void
get_left_vec(const math::transform *transform, math::vec3 *out_vec3)
{
  // Param check.
  assert(transform && out_vec3);
  
  *out_vec3 = math::quat_rotate_point(transform->rotation, world_left());
}


void
get_up_vec(const math::transform *transform, math::vec3 *out_vec3)
{
  // Param check.
  assert(transform && out_vec3);
  
  *out_vec3 = math::quat_rotate_point(transform->rotation, world_up());
}


} // ns