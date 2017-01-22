#include <transformations/entity/entity_common.hpp>
#include <data/world_data.hpp>
#include <data/world/pending_entity_removal_data.hpp>
//#include <data/world/entity_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>


namespace Entity_detail {


util::generic_id
get_id(const util::generic_id this_id,
       Data::Entity_data *entity_data)
{
  return is_valid(this_id, entity_data) ? this_id : util::generic_id_invalid();
}


bool
is_valid(const util::generic_id this_id,
         Data::Entity_data *entity_data,
         const bool emit_error)
{
  // Invalid data, means invalid entity.
  if(this_id == util::generic_id_invalid()) { return false; }
  if(!entity_data)                          { return false; }

  // Our id might have expired, so we need to check it.
  Data::data_lock(entity_data);
  const bool exists = Data::entity_exists(entity_data, this_id);
  Data::data_unlock(entity_data);
  
  if(!exists && emit_error)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
  }
  
//  return exists;
  return true;
}


void
destroy(const util::generic_id this_id,
        Data::Entity_data *entity_data,
        Data::Pending_entity_removal_data *scene_graph_changes)
{
  if(!is_valid(this_id, entity_data, false)) {
    return; // No assert, destroying an invalid entity shouldn't result in chaos.
  }
  
  // Add entity to the destruction list.
  Data::data_lock(scene_graph_changes);
  
  const uint32_t removal_id = Data::pending_entity_removal_push(scene_graph_changes);
  Data::pending_entity_removal_set_deleted_entity(scene_graph_changes, removal_id, &this_id);
  
  Data::data_unlock(scene_graph_changes);
}


void
update_component(const util::generic_id this_id,
                 Data::Entity_data *entity_data,
                 const uint32_t component_id)
{
  if(!is_valid(this_id, entity_data, true)) {
    assert(false); return;
  }
 
  Data::data_lock(entity_data);

  uint32_t comps;
  Data::entity_get_components(entity_data, this_id, &comps);

  comps = comps | component_id;
  Data::entity_set_components(entity_data, this_id, &comps);
  
  Data::data_unlock(entity_data);
}


} // ns