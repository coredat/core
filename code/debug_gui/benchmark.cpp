#include "benchmark.hpp"
#include <common/error_strings.hpp>
#include <utilities/logging.hpp>


namespace
{
  struct Bench
  {
    char      name[32];
    uint32_t  ms;
    uint32_t  thread_id;
  };
  
  uint32_t benchmark_size = 0;
  constexpr uint32_t benchmark_capacity = 32;
  Bench benchmarks[benchmark_capacity];
}


namespace Benchmark {


uint32_t
add_stat(const char *name)
{
  if(benchmark_size >= benchmark_capacity)
  {
    LOG_ERROR(Error_string::no_free_space());
    return 0;
  }
  
  strlcat(benc, <#const char *#>, <#size_t#>)
  
  ++benchmark_size;
  
  return benchmark_size;
}


void
udpate_stat(const uint32_t stat_id, const uint32_t ms, const uint32_t thread_id)
{
}


} // ns


namespace Debug_gui {


uint32_t
add_stat(const char *name)
{
  return 0;
}


} // ns