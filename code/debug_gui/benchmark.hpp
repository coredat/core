#ifndef BENCHMARK_INCLUDED_8B8543ED_09D5_445A_9557_A46882DE55E9
#define BENCHMARK_INCLUDED_8B8543ED_09D5_445A_9557_A46882DE55E9


#include <stdint.h>


#define BENCHMARK(name)


namespace Benchmark {


uint32_t
add_stat(const char *name);


void
udpate_stat(const uint32_t stat_id, const uint32_t ms, const uint32_t thread_id);



} // ns


/*
  Display the benchmark
*/
namespace Debug_gui {



} // ns


#endif // inc guard