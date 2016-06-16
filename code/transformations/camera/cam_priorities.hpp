#ifndef CAM_UTILS_INCLUDED_24851027_2A53_4F2A_88FA_93B9B5FC7810
#define CAM_UTILS_INCLUDED_24851027_2A53_4F2A_88FA_93B9B5FC7810


#include <stdint.h>
#include <stddef.h>


namespace Camera_utils {


uint32_t
find_highest_priority(const uint32_t priorities[],
                      const uint32_t count);


size_t
find_insert_point_based_on_priority(const uint32_t priority,
                                    const uint32_t priorities[],
                                    const uint32_t count);


} // ns


#endif // inc guard