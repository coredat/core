#ifndef MATERIAL_RENDERER_INCLUDED_40B58250_DD62_4792_9DE7_554E728832C5
#define MATERIAL_RENDERER_INCLUDED_40B58250_DD62_4792_9DE7_554E728832C5


#include <data/global_data/material_data.hpp>
#include <data/global_data/mesh_data.hpp>
#include <data/world_data/renderer_mesh_data.hpp>
#include <data/world_data/transform_data.hpp>
#include <math/mat/mat_types.hpp>


namespace Rendering {


/*
  Renders the materials to the currently bound target.
  This method will allocate data onto the scratch buffer.
  \param view_mat The view matrix.
  \param proj_mat The projection matrix.
  \param material_data The global material data store.
  \param mesh_data The global mesh data store.
  \param render_mesh_data The draw calls information.
  \param transforms The entity transforms.
  \return Returns the number of draw calls it will execute.
*/
uint32_t
material_renderer(const math::mat4 &view_mat,
                  const math::mat4 &proj_mat,
                  const Resource_data::Material_data *material_data,
                  const Resource_data::Mesh_data *mesh_data,
                  const World_data::Renderer_mesh_data *render_mesh_data);


}


#endif // inc guard