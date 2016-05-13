#ifndef SCENE_GRAPH_CHANGES_INCLUDED_320B69B4_7593_45C5_BE1D_A927BD8F1C87
#define SCENE_GRAPH_CHANGES_INCLUDED_320B69B4_7593_45C5_BE1D_A927BD8F1C87


#include <utilities/generic_id.hpp>
#include <stdint.h>


namespace World_data {


struct Scene_graph_changes
{
  util::generic_id    *entities_ids;
  
  uint32_t            size = 0;
  const uint32_t      capacity = 0;
};


void
lock(Scene_graph_changes *graph_changes);


void
unlock(Scene_graph_changes *graph_unlock);


void
scene_graph_init(Scene_graph_changes *graph_changes, const uint32_t size_hint);


void
scene_graph_reset(Scene_graph_changes *graph);


void
scene_graph_changes_delete(Scene_graph_changes *graph_changes, const util::generic_id id);


} // ns


#endif // inc guard