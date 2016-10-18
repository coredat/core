#ifndef RENDER_SCENE_INCLUDED_3E7A0E7E_BC80_47F5_9A8F_2F35E0FE88AB
#define RENDER_SCENE_INCLUDED_3E7A0E7E_BC80_47F5_9A8F_2F35E0FE88AB


#include <transformations/camera/cam_priorities.hpp>
#include <systems/renderer_material/material_renderer.hpp>
#include <data/fwd_dec_collections.hpp>
#include <data/fwd_dec_data.hpp>
#include <stdint.h>


namespace Rendering {


/*!
  This will call the various renderers.
*/
void
render_main_scene(const float delta_time,
                  const float total_time,
                  const uint32_t viewport_x,
                  const uint32_t viewport_y,
                  const Data::World                      *world,
                  const Data::Material_data     *material_data,
                  const Data::Post_process_data *post_data,
                  const Camera_utils::Cam_run            cam_runs[],
                  const uint32_t                         number_of_cam_runs,
                  const Material_renderer::Draw_call     draw_calls[],
                  const uint32_t                         number_of_draws,
                  uint32_t                               *out_draw_calls);


} // ns


#endif // inc guard