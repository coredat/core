#include <data/world_data/pending_scene_graph_change_data.hpp>
#include <cstring>
#include <assert.h>


namespace World_data {


void
lock(Pending_scene_graph_change_data *graph_changes)
{
}


void
unlock(Pending_scene_graph_change_data *graph_unlock)
{
}


void
pending_scene_graph_change_init(Pending_scene_graph_change_data *graph_changes, const uint32_t size_hint)
{
  assert(graph_changes && size_hint);

  lock(graph_changes);
  
  static util::generic_id ids[2048];
  
  #ifndef NDEBUG
  memset(ids, 0, sizeof(ids));
  #endif
  
  graph_changes->entities_to_delete = ids;
  
  uint32_t *cap = const_cast<uint32_t*>(&graph_changes->capacity);
  *cap = 2048;
  
  unlock(graph_changes);
}


void
pending_scene_graph_change_reset(Pending_scene_graph_change_data *graph)
{
  assert(graph);
  
  lock(graph);

  #ifndef NDEBUG
  memset(graph->entities_to_delete, 0, sizeof(util::generic_id) * graph->delete_size);
  #endif
  
  graph->delete_size = 0;
  
  unlock(graph);
}


void
pending_scene_graph_change_delete(Pending_scene_graph_change_data *graph_changes, const util::generic_id id)
{
  assert(graph_changes && id);
  
  lock(graph_changes);
  
  graph_changes->entities_to_delete[graph_changes->delete_size] = id;
  ++(graph_changes->delete_size);
  
  unlock(graph_changes);
}


} // ns