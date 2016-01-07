#ifndef ENTITY_ID_045C8238_F1E0_4950_8704_3107E71D6C3C
#define ENTITY_ID_045C8238_F1E0_4950_8704_3107E71D6C3C


#include <stdint.h>
#include <cstddef>


namespace Entity {


struct Entity_id
{
  uint32_t type_id : 8;
  uint32_t instance_id : 24;
}; // Entity_id



constexpr bool
operator==(const Entity_id left, const Entity_id right)
{
  return ((left.type_id == right.type_id) && (left.instance_id == right.instance_id));
}



constexpr bool
operator<(const Entity_id left, const Entity_id right)
{
  return ((left.type_id < right.type_id) && (left.instance_id < right.instance_id));
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



void
init_to_invalid_ids(Entity_id ids[], const std::size_t size_of_entities);


/*!
  Search through a list of id's to find a match linearly.
  \param out_index pointer to the result.
  \param id_to_find the id to look for.
  \param ids[] array of ids.
  \param number_of_entities length of the array.
  \return return true if an index has been found.
*/
bool
find_index_linearly(std::size_t *out_index, const Entity_id id_to_find, const Entity_id ids[], const std::size_t number_of_entities);



} // ns



#endif // include