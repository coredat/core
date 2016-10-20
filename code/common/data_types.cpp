#include <common/data_types.hpp>


namespace Common {
namespace Data_type {


bool
has_data_type(const uint32_t data_types, const uint32_t type)
{
  return (data_types & type) > 0;
}


bool
is_collidable(const uint32_t data_types)
{
  return has_data_type(data_types, Data_type::rigidbody) |
         has_data_type(data_types, Data_type::trigger);
}


bool
is_renderable(const uint32_t data_types)
{
  return has_data_type(data_types, Data_type::renderer_mesh) |
         has_data_type(data_types, Data_type::renderer_text);
}


} // ns
} // ns