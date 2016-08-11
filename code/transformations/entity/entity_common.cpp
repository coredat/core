#include <transformations/entity/entity_common.hpp>
#include <data/world_data/world_data.hpp>
#include <data/world_data/entity_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>


namespace Entity_detail {


util::generic_id
get_id(const util::generic_id this_id,
       World_data::Entity_data *entity_data)
{
  return is_valid(this_id, entity_data) ? this_id : util::generic_id_invalid();
}


bool
is_valid(const util::generic_id this_id,
         World_data::Entity_data *entity_data,
         const bool emit_error)
{
  // Invalid data, means invalid entity.
  if(this_id == util::generic_id_invalid()) { return false; }
  if(!entity_data)                          { return false; }

  // Our id might have expired, so we need to check it.
  World_data::data_lock(entity_data);
  const bool exists = World_data::entity_data_exists(entity_data, this_id);
  World_data::data_unlock(entity_data);
  
  if(!exists && emit_error)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
  }
  
  return exists;
}


void
destroy(const util::generic_id this_id,
        World_data::Entity_data *entity_data,
        World_data::Pending_scene_graph_change_data *scene_graph_changes)
{
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }
  
  World_data::pending_scene_graph_change_delete(scene_graph_changes, this_id);
}


void
update_component(const util::generic_id this_id,
                 World_data::Entity_data *entity_data,
                 const uint32_t component_id)
{
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }
 
  World_data::data_lock(entity_data);

  uint32_t comps;
  World_data::entity_data_get_property_components(entity_data, this_id, &comps);

  comps = comps | component_id;
  World_data::entity_data_set_property_components(entity_data, this_id, comps);
  
  World_data::data_unlock(entity_data);
}


} // ns