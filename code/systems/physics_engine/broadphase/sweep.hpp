#ifndef SWEEP_INCLUDED_02F9E223_6037_4E44_890E_E3A89918F454
#define SWEEP_INCLUDED_02F9E223_6037_4E44_890E_E3A89918F454


#include <math/geometry/geometry_types.hpp>
#include <stdint.h>


/*!
 * Sweep is part of a broadphase processes (see prune for the other part).
 * This quickily projects bounding volumes onto axis's so that prune
 * can discard objects quickily.
 */


namespace Physics {
namespace Broadphase {


/*
 * Sweep_axis represents a position on an axis, with a half extents,
 * this is used to easily see if pairs is overlapping.
 */
struct Sweep_axis
{
  float center;
  float half_extent;
};


/*
 * Sweep contains the pairs in for each axis.
 */
struct Sweep
{
  // Arrays to the projected axis information.
  Sweep_axis *x_axis;
  Sweep_axis *y_axis;
  Sweep_axis *z_axis;

  uint32_t size = 0; 
};


/*!
 * Allocates memory for the sweep.
 * \param sweep The structure to store the results in.
 * \param number_of_bounds How many pairs to allocate in memory.
 */
void
sweep_init(Sweep *sweep, const uint32_t number_of_bounds);


/*!
 *  Sweeps the aabb you've given it, and produces the details
 *  per axis. Which can be used for the prune stage.
 *  The order of the inputed bounding aabbs will match the
 *  order of the output sweep axis information.
 */
void
sweep_calculate(Sweep *sweep,
                const math::aabb bounding_area[],
                const uint32_t number_of_aabbs);


/*!
 * Get the number of items in the sweep.
 */
uint32_t
sweep_get_size(const Sweep *sweep);


/*!
 * Free any memory that needs to cleared.
 */
void
sweep_free(Sweep *sweep);


} // ns
} // ns


#endif // 
