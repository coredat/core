#include <transformations/entity/entity_common.hpp>
#include <data/world_data/world_data.hpp>
#include <data/world_data/entity_data.hpp>
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>


namespace Entity_detail {


util::generic_id
get_id(const util::generic_id this_id,
       World_data::World *world)
{
  return is_valid(this_id, world) ? this_id : util::generic_id_invalid();
}


bool
is_valid(const util::generic_id this_id,
         World_data::World *world,
         const bool emit_error)
{
  // Invalid data, means invalid entity.
  if(this_id == util::generic_id_invalid()) { return false; }
  if(!world)                                { return false; }

  // Our id might have expired, so we need to check it.
  World_data::data_lock(world->entity);
  const bool exists = World_data::entity_data_exists(world->entity, this_id);
  World_data::data_unlock(world->entity);
  
  if(!exists && emit_error)
  {
    LOG_ERROR(Error_string::entity_is_invalid());
  }
  
  return exists;
}


void
destroy(const util::generic_id this_id,
        World_data::World *world)
{
  if(!is_valid(this_id, world)) { return; }
  
  // Destroy this.
  World_data::pending_scene_graph_change_delete(world->entity_graph_changes, this_id);
}


void
update_component(const util::generic_id this_id,
                 World_data::World *data,
                 const uint32_t component_id)
{
  auto entity_data = data->entity;
  
  World_data::data_lock(entity_data);

  uint32_t comps;
  World_data::entity_data_get_property_components(entity_data, this_id, &comps);

  // Add new component_id
  comps = comps | component_id;

  World_data::entity_data_set_property_components(entity_data, this_id, comps);
  
  World_data::data_unlock(entity_data);
}


} // ns