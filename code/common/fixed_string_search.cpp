#include <common/fixed_string_search.hpp>
#include <utilities/string_helpers.hpp>


namespace Common {


bool
fixed_string_search(const char *string_to_find,
                    const char *string_data,
                    const size_t size_of_string,
                    const size_t number_of_strings,
                    size_t *out_count)
{
  bool found = false;
  
  for(size_t i = 0; i < number_of_strings; ++i)
  {
    if(!strcmp(string_to_find, &string_data[i * size_of_string]))
    {
      found = true;
      
      if(out_count)
      {
        *out_count = i;
      }
      
      break;
    }
  }
  
  return found;
}
  
  
} // ns