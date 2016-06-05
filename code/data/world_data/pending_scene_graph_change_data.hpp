#ifndef SCENE_GRAPH_CHANGES_INCLUDED_320B69B4_7593_45C5_BE1D_A927BD8F1C87
#define SCENE_GRAPH_CHANGES_INCLUDED_320B69B4_7593_45C5_BE1D_A927BD8F1C87


#include <utilities/generic_id.hpp>
#include <stdint.h>


namespace World_data {


/*!
  Tracks changes to the scene graph.
  Right now we only care about entities that should be deleted.
*/
struct Pending_scene_graph_change_data
{
  util::generic_id    *entities_to_delete = nullptr;
  uint32_t            delete_size         = 0;
  
  const uint32_t      capacity            = 0;
};


void
lock(Pending_scene_graph_change_data *graph_changes);


void
unlock(Pending_scene_graph_change_data *graph_unlock);


void
pending_scene_graph_change_init(Pending_scene_graph_change_data *graph_changes,
                                const uint32_t size_hint);


void
pending_scene_graph_change_reset(Pending_scene_graph_change_data *graph);


void
pending_scene_graph_change_delete(Pending_scene_graph_change_data *graph_changes,
                                  const util::generic_id id);


} // ns


#endif // inc guard