#ifndef FIXED_STRING_SEARCH_INCLUDED_18B1F6BA_65E5_4942_B9E1_B2B3296FCA24
#define FIXED_STRING_SEARCH_INCLUDED_18B1F6BA_65E5_4942_B9E1_B2B3296FCA24


#include <stddef.h>


namespace Common {


bool
fixed_string_search(const char *string_to_find,
                    const char *string_data,
                    const size_t size_of_string,
                    const size_t number_of_strings,
                    size_t *out_count = nullptr);


};


#endif // inc guard