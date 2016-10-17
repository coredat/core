#ifndef MATERIAL_RENDERER_INCLUDED_40B58250_DD62_4792_9DE7_554E728832C5
#define MATERIAL_RENDERER_INCLUDED_40B58250_DD62_4792_9DE7_554E728832C5


#include <data/global_data/material_data.hpp>
#include <data/world_data/mesh_draw_call_data.hpp>
#include <systems/renderer_material/material.hpp>
#include <systems/renderer_material/material_renderer.hpp>
#include <math/mat/mat_types.hpp>


namespace Rendering {


/*
  Renders the materials to the currently bound target.
  This method will allocate data onto the scratch buffer.
  \param view_mat The view matrix.
  \param proj_mat The projection matrix.
  \param material_data The global material data store.
  \param camera_cull_mask Is so certain meshes can be drawn by certain cameras.
  \param render_mesh_data The draw calls information.
  \param draw_calls The draws calls in material order
  \param number_of_draw_calls The number of draw calls.
  \return Returns the number of draw calls it will execute.
*/
uint32_t
material_renderer(const math::mat4 &view_mat,
                  const math::mat4 &proj_mat,
                  const float delta_time,
                  const float total_time,
                  const Resource_data::Material_data *material_data,
                  const uint32_t camera_cull_mask,
                  const Data::Mesh_draw_call_data *mesh_renderer_data,
                  const ::Material_renderer::Draw_call *draw_calls,
                  const uint32_t number_of_draw_calls);


}


#endif // inc guard