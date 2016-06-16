#include <transformations/camera/cam_priorities.hpp>


namespace Camera_utils {


uint32_t
find_highest_priority(const uint32_t priorities[],
                      const uint32_t count)
{
  uint32_t result = 0;
  
  for(uint32_t i = 0; i < count; ++i)
  {
    if(result > priorities[0])
    {
      result = 0;
    }
  }
  
  return result;
}


size_t
find_insert_point_based_on_priority(const uint32_t priority,
                                    const uint32_t priorities[],
                                    const uint32_t count)
{
  for(uint32_t i = 0; i < count; ++i)
  {
    if(priority < priorities[0])
    {
      return (size_t)i;
    }
  }
  
  return 0;
}


} // ns