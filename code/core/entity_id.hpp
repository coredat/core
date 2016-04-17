#ifndef ENTITY_ID_045C8238_F1E0_4950_8704_3107E71D6C3C
#define ENTITY_ID_045C8238_F1E0_4950_8704_3107E71D6C3C


#include <stdint.h>
#include <stddef.h>


/*
  TODO: Move this down to uitls.
  call it generic_id or something.
  
  TODO: Move more funcs to cpp file.
*/


namespace Core {


struct Entity_id
{
  uint32_t type_id : 8;
  uint32_t instance_id : 24;
}; // Entity_id


namespace Entity_id_util {


//! Converts an entity type into a uint32_t type.
inline uint32_t
convert_entity_to_uint(const Entity_id e)
{
  return (e.type_id << 24) | e.instance_id;
}


//! Converts a uint to an entity
inline Entity_id
convert_uint_to_entity(const uint32_t to_e)
{
  return Entity_id{(to_e >> 24), (to_e >> 0 & 0xFFFF)};
}


inline void*
convert_entity_to_ptr(const Entity_id to_ptr)
{
  const uint32_t usr = convert_entity_to_uint(to_ptr);
  const uintptr_t warn = static_cast<uintptr_t>(usr);
  uint32_t* ptr = nullptr;
  
  ptr = (uint32_t*)warn;
  
  return ptr;
}


inline Entity_id
convert_ptr_to_entity(const void *to_ent)
{
  const uint32_t ptr = (size_t)to_ent;
  const Core::Entity_id id = convert_uint_to_entity(static_cast<uint32_t>(ptr));
  
  return id;
}


constexpr bool
is_valid(const Entity_id id)
{
  return id.type_id == 0;
}


constexpr Entity_id
invalid_id()
{
  return Entity_id{0,0};
};


/*!
 Sets an array of ids to invalid.
 \param ids[] ids to get set to invalid.
 \param size_of_entities the number of entities to invalidate.
*/
void
init_to_invalid_ids(Entity_id ids[], const uint32_t size_of_entities);


/*!
  Search through a list of id's to find a match linearly.
  \param out_index pointer to the result.
  \param id_to_find the id to look for.
  \param ids[] array of ids.
  \param number_of_entities length of the array.
  \return return true if an index has been found.
*/
bool
find_index_linearly(uint32_t *out_index, const Entity_id id_to_find, const Entity_id ids[], const uint32_t number_of_entities);


} // ns
} // ns


constexpr bool
operator==(const Core::Entity_id left, const Core::Entity_id right)
{
  return ((left.type_id == right.type_id) && (left.instance_id == right.instance_id));
}


constexpr bool
operator!=(const Core::Entity_id left, const Core::Entity_id right)
{
  return !operator==(left, right);
}


constexpr bool
operator<(const Core::Entity_id left, const Core::Entity_id right)
{
  return ((left.type_id < right.type_id) && (left.instance_id < right.instance_id));
}


#endif // include