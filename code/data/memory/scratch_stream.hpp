#ifndef SCRATCH_STREAM_INCLUDED_5C875010_0E64_4B7F_A9A5_0CAE7B77A5E6
#define SCRATCH_STREAM_INCLUDED_5C875010_0E64_4B7F_A9A5_0CAE7B77A5E6


#include <stddef.h>


namespace Memory {
namespace Scratch_stream {


void
initialize(void *start_of_data, const size_t size_of_pool);


void
reset();


void*
open();


bool
add(const void *data, const size_t bytes_of_data);


void
close();


} // ns
} // ns


#endif // inc guard