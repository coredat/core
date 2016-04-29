#include "prune.hpp"
#include <assert.h>
#include "sweep.hpp"


namespace Physics {
namespace Broadphase {


void
prune_init(Prune *prune, const uint32_t size_hint)
{
  assert(init);

  prune->ids = new uint32_t[size_hint];
}


void
prune_calculate(Prune *prune, const Sweep *sweep)
{
}


void
prune_free(Prune *prune)
{
  assert(prune);

  delete[] prune->ids;
}


} // ns
} // ns
