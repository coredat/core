#ifndef PRUNE_INCLUDED_8DB46A86_EFF1_4497_A1AB_76EEF2D51F97
#define PRUNE_INCLUDED_8DB46A86_EFF1_4497_A1AB_76EEF2D51F97


#include <stdint.h>
#include "broadphase_fwd.hpp"


namespace Physics {
namespace Broadphase {


struct Prune
{
  uint32_t *ids; // array of overlapping index.
  uint32_t size = 0;
  uint32_t capacity = 0;
};


/*!
 * Allocates the memory required for the prune method.
 */
void
prune_init(Prune *init, const Sweep *sweep);


/*!
 * Prune will remove out obects that are not overlapping.
 * Prune will store a selection of ids that could possibly
 * be overlapping.
 */
void
prune_calculate(Prune *prune, const Sweep *sweep);


/*!
 *  Free the memory allocated to this object.
 */
void
prune_free(Prune *prune);


} // ns
} // ns


#endif // inc guard
