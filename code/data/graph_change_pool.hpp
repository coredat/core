#ifndef GRAPH_CHANGES_INCLUDED_3E94C537_4E59_4CA4_8A58_BC5487EFF734
#define GRAPH_CHANGES_INCLUDED_3E94C537_4E59_4CA4_8A58_BC5487EFF734


#define GRAPH_CHANGE_POOL 128


#include <core/entity_id.hpp>


namespace Data {


/*!
  Entities that have 'changed' in the tree in the last frame.
*/
enum class Entity_graph_change
{
  inserted,
  moved,
  updated,
  removed,
};


struct Entity_change
{
  Entity_graph_change change_type;
  Core::Entity_id entity_id;
};


struct Entity_graph_changes_pool
{
  Entity_change       entity_event[GRAPH_CHANGE_POOL];
  size_t              size;
  const size_t        capacity = GRAPH_CHANGE_POOL;
};


/*!
  Graph changes events
*/
void
entity_graph_change_pool_init(Entity_graph_changes_pool *pool);


/*!
  Push pending graph changes in
*/
void
entity_graph_change_push(Entity_graph_changes_pool *pool, const Core::Entity_id entity_id, Entity_graph_change change_type);


} // ns


#endif // inc guard