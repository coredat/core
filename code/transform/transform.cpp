#include "transform.hpp"


namespace Transform {


void
transforms_to_wvp(const math::transform input_transforms[],
                  const std::size_t number_of_input_transforms,
                  const math::mat4 proj_mat,
                  const math::mat4 view_mat,
                  void  *output_mats,
                  const std::size_t number_of_output_mats,
                  const std::size_t byte_stride)
                  {
                    const uint32_t number_to_process = math::min(number_of_input_transforms, number_of_output_mats);
                    const uint32_t floats_in_mat4 = 16;
                  
                    for(std::size_t i = 0; i < number_to_process; ++i)
                    {
                      // Calculate wvp matrix.
                      const math::mat4 world_mat = math::transform_get_world_matrix(input_transforms[i]);
                      const math::mat4 wvp_mat = math::mat4_multiply(world_mat, view_mat, proj_mat);
                      
                      // Starting point taking into account stride.
                      const std::size_t stride = i * byte_stride;
                      uint8_t *data = &reinterpret_cast<uint8_t*>(output_mats)[stride];
                      
                      // Insert point after stride taken into account.
                      const std::size_t index = i * (sizeof(float) * floats_in_mat4);
                      float *out_data = reinterpret_cast<float*>(&data[index]);
                      
                      // Insert result into output.
                      math::mat4_to_array(wvp_mat, out_data);
                    }
                  }



} // ns