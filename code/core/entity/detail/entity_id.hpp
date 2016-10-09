#ifndef ENTITY_ID_INCLUDED_D258FF69_D7AE_428E_BA71_5730065A7365
#define ENTITY_ID_INCLUDED_D258FF69_D7AE_428E_BA71_5730065A7365


#include <core/common/core_types.hpp>


namespace Core_detail {


constexpr uint32_t entity_detail_world_bits()   { return 8;  }
constexpr uint32_t entity_detail_entity_bits()  { return 24; }


/*
  Entity id is a uint32_t that combines both the world instance and entity instance.
*/
struct Entity_id
{
  uint32_t world_instance : entity_detail_world_bits();
  uint32_t entity_instance : entity_detail_entity_bits();
};


Entity_id
entity_id_create(const uint32_t id, const uint32_t world_id);


bool
entity_id_is_valid(const Entity_id id);


Entity_id
entity_id_invalid();


uint32_t
entity_id_to_uint(const Entity_id id);


Entity_id
entity_id_from_uint(const uint32_t id);


} // ns


#endif // inc guard