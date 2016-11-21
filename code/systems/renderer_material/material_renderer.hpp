#ifndef MATERIAL_RENDERER_INCLUDED_8DD3647B_EE2F_4233_95CC_3533DB86CC5E
#define MATERIAL_RENDERER_INCLUDED_8DD3647B_EE2F_4233_95CC_3533DB86CC5E


#include <systems/renderer_material/material.hpp> // Can be fwd declared
#include <graphics_api/mesh.hpp>
#include <math/mat/mat_types.hpp>
#include <math/vec/vec_types.hpp>
#include <stdint.h>


namespace Material_renderer {


struct Draw_call
{
  float               world_matrix[16];
  Graphics_api::Mesh  mesh;
  uint32_t            cull_mask = -1;
};


/*
  Initializes the material renderer.
*/
void
initialize();


/*
  Resets the internal cache, which allows for some 
  minor reduction in state changes.
  
  Best to call this once per frame, just before you are rendering
  your materials.
*/
void
reset();


/*
  Renders an array of meshes with a given material.
  \returns The number of draw calls that happend.
*/
uint32_t
render(const math::mat4 &view_proj_mat,
       const math::vec3 eye_pos,
       const float delta_time,
       const float total_time,
       const Material *material,
       const uint32_t cull_mask,
       const Draw_call calls[],
       const uint32_t number_of_calls);


} // ns


#endif // inc guard