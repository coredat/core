#ifndef SCRATCH_STREAM_INCLUDED_5C875010_0E64_4B7F_A9A5_0CAE7B77A5E6
#define SCRATCH_STREAM_INCLUDED_5C875010_0E64_4B7F_A9A5_0CAE7B77A5E6


namespace Data {
namespace Memory {
namespace Scratch_stream {


class Stream_lock
{
public:
};


void
initialize(const size_t bytes_to_reserve);


void
free();


Stream_lock
open_stream();


bool
add_data(const Stream_lock *lock, const void *data, const size_t size_of_data);




} // ns
} // ns
} // ns


#endif // inc guard