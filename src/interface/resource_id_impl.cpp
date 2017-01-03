#include "resource_id_impl.hpp"


namespace op {


uint32_t
generate_id(const uint8_t type_id, const uint32_t instance_id)
{
  return (type_id << 24) | instance_id;
}


uint8_t
type_id(const uint32_t id)
{
  return id >> 24;
}


uint32_t
instance_id(const uint32_t id)
{
  return id & 0xFFFFFF;
}


bool
resource_is_valid(const uint32_t id)
{
  return !!type_id(id);
}



} // ns
