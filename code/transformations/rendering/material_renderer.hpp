#ifndef MATERIAL_RENDERER_INCLUDED_40B58250_DD62_4792_9DE7_554E728832C5
#define MATERIAL_RENDERER_INCLUDED_40B58250_DD62_4792_9DE7_554E728832C5


#include <data/global_data/material_data.hpp>
#include <data/global_data/mesh_data.hpp>
#include <data/world_data/renderer_mesh_data.hpp>
#include <data/world_data/transform_data.hpp>
#include <math/mat/mat_types.hpp>


namespace Rendering {


void
material_renderer(const math::mat4 &view_mat,
                  const math::mat4 &proj_mat,
                  const Resource_data::Material_data *material_data,
                  const Resource_data::Mesh_data *mesh_data,
                  const World_data::Renderer_mesh_data *render_mesh_data,
                  const World_data::Transform_data *transforms);


}


#endif // inc guard