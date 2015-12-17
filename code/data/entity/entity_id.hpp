#ifndef ENTITY_ID_
#define ENTITY_ID_


#include <stdint.h>


struct Entity_id
{
  uint32_t type_id : 8;
  uint32_t instance_id : 24;
}; // entity


inline bool operator==(const Entity_id left, const Entity_id right)
{
  return ((left.type_id == right.type_id) && (left.instance_id == right.instance_id));
}



inline bool operator<(const Entity_id left, const Entity_id right)
{
  return ((left.type_id < right.type_id) && (left.instance_id < right.instance_id));
}






#endif // include