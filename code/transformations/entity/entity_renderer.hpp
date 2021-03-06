#ifndef ENTITY_RENDERER_INCLUDED_B672FA56_D1B2_444E_BD28_F0ED3138E2F0
#define ENTITY_RENDERER_INCLUDED_B672FA56_D1B2_444E_BD28_F0ED3138E2F0


#include <transformations/entity/entity_common.hpp>
#include <data/graph/fwd.hpp>


namespace Entity_detail {


uint32_t
has_renderer(const uint32_t this_id,
             Data::Graph::Graph_data *entity_data);


void
set_renderer(const uint32_t this_id,
             Data::Graph::Graph_data *entity_data,
             Data::Mesh_draw_call_data *mesh_data,
             Data::Text_draw_call_data *text_data,
             const Core::Renderer &renderer);
  
  
Core::Renderer
get_renderer(const uint32_t this_id,
             Data::Graph::Graph_data *entity_data,
             Data::Mesh_draw_call_data *renderer_material,
             Data::Text_draw_call_data *text_draw_call_data);


void
set_renderer_material(const uint32_t this_id,
                      Data::Graph::Graph_data *entity_data,
                      Data::Mesh_draw_call_data *mesh_data,
                      const uint32_t material_id,
                      const uint32_t model_id);
  

void
get_renderer_material(const uint32_t this_id,
                      Data::Mesh_draw_call_data *mesh_data,
                      uint32_t *out_material_id,
                      uint32_t *out_model_id);

  
void
set_renderer_text(const uint32_t this_id,
                  const char *string,
                  Data::Graph::Graph_data *entity_data,
                  Data::Text_draw_call_data *text_data,
                  const uint32_t font_id,
                  const uint32_t model_id);


} // ns


#endif // inc guard
