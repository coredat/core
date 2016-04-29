#ifndef PRUNE_INCLUDED_8DB46A86_EFF1_4497_A1AB_76EEF2D51F97
#define PRUNE_INCLUDED_8DB46A86_EFF1_4497_A1AB_76EEF2D51F97


#include <stdint.h>
#include "sweep.hpp" // use fwd dec file.


namespace Physics {
namespace Broadphase {


struct Prune
{
  uint32_t *ids; // array of overlapping index.
  uint32_t size = 0;
  uint32_t capacity = 0;
}


/*!
 * Allocates memory for the results.
 * You should apply a hint to this, a good hint
 * would be the number of bounding boxes you gave
 * to the sweep method.
 */
void
prune_init(Prune *init, const uint32_t hint);


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
