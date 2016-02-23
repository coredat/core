#ifndef MESH_DATA_INCLUDED_A8754D41_F518_4378_A1D4_7B46EEE7387D
#define MESH_DATA_INCLUDED_A8754D41_F518_4378_A1D4_7B46EEE7387D


#include <renderer/graphics_api/ogl/ogl_vertex_buffer.hpp>
#include <math/geometry/geometry.hpp>
#include <stddef.h>
#include <stdint.h>


namespace Model {


struct Load_model
{
  const size_t id;         //!< Index into the output.
  const std::string file_name;  //!< File to load
}; // struct


/*!
  Loads models into output array, using the given index (id) provided in Load_model.
  \param model_to_load[] array of Load_model with details of things to load.
  \param number_of_models_to_load size of the model_to_load array.
  \param output_vbo[] array of vbo's that get created.
  \param size_of_output_pool size of output_vbo array.
*/
void
model_load(const Load_model models_to_load[],
           const size_t number_of_models_to_load,
           Ogl::Vertex_buffer output_vbo[],
           math::aabb output_aabb[],
           const std::size_t size_of_output_pool);

  
void
find_model_data(Ogl::Vertex_buffer output_vbo[],
                const size_t size_of_output,
                const uint32_t input_ids[],
                const Ogl::Vertex_buffer input_vbo[],
                const size_t size_of_input);



} // ns


#endif // inc guard