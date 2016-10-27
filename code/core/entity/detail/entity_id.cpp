#include <core/entity/detail/entity_id.hpp>
#include <utilities/bits.hpp>


namespace Core_detail {


Entity_id
entity_id_create(const uint32_t entity_id, const uint32_t world_id)
{
  static_assert((entity_detail_entity_bits() + entity_detail_world_bits()) == (sizeof(Entity_id) * CHAR_BIT), "Combined bits must match bits available in Entity_id.");

  Entity_id ent_id;
  ent_id.entity_instance = (entity_id >> entity_detail_world_bits()) & 0xFFFF;
  ent_id.world_instance = world_id;
  
  return ent_id;
}


bool
entity_id_is_valid(const Entity_id id)
{
  return id.world_instance > 0 && id.entity_instance > 0;
}


Entity_id
entity_id_invalid()
{
  Entity_id id;
  id.world_instance = 0;
  id.entity_instance = 0;
  
  return id;
}


uint32_t
entity_id_to_uint(const Entity_id id)
{
  return static_cast<uint32_t>((id.entity_instance << entity_detail_world_bits()) | (id.world_instance));
}


Entity_id
entity_id_from_uint(const uint32_t int_id)
{
  Entity_id id;
  
  id.entity_instance = int_id >> entity_detail_world_bits();
  id.world_instance = int_id & 0xFF;
  
  return id;
}


} // ns