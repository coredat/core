#include "prune.hpp"
#include <assert.h>
#include "sweep.hpp"
#include <math/general/general.hpp>


namespace Physics {
namespace Broadphase {


void
prune_init(Prune *prune, const uint32_t size_hint)
{
  assert(prune);

  prune->ids = new uint32_t[size_hint];
}


void
prune_calculate(Prune *prune, const Sweep *sweep)
{
  // For each axis prune out objects that really can't be touching.
  constexpr uint32_t number_of_axis = 3;
  const Sweep_axis *axis[number_of_axis] = {sweep->x_axis, sweep->y_axis, sweep->z_axis};

  for(uint32_t a = 0; a < number_of_axis; ++a)
  {
    // Loop through each item and check if it collides with something on that axis.
    uint32_t obj_id = 0;
    bool potential_collision = false;

    while(obj_id < sweep->size)
    {
      const Sweep_axis *test_obj = &axis[a][obj_id];

      uint32_t sweep_id = 0;

      while(sweep_id < sweep->size)
      {
        // Don't self test
        if(sweep_id == obj_id)
        {
          ++sweep_id;
          continue;
        }

        // Test if this object is overlapping
        const Sweep_axis *sweep_obj = &axis[a][sweep_id];
        
        const float distance = math::abs(test_obj->center - sweep_obj->center);        
        const float combined_extent = test_obj->half_extent + sweep_obj->half_extent;

        // If distance < combined_extent
        // then they are colliding.
        if(distance < combined_extent)
        {
          // No point testing this object
          // anymore we know its potentially
          // colliding.
          potential_collision = true;
          break;
        }

        ++sweep_id;
      }

      // This can be pruned out.
      if(!potential_collision)
      {
        prune->ids[prune->size++] = obj_id;
      }

      ++obj_id;
    }
  } // for axis

  // Now we have a list of objects that are not colliding
  // we might possibly have duplicates so lets remove those.
  {
    uint32_t prune_index = 0;

    while(prune_index < prune->size)
    {
      const uint32_t id_to_search = prune->ids[prune_index];

      uint32_t search_index = 0;
      while(search_index < prune->size)
      {
        // Don't self check
        if(prune_index == search_index)
        {
          ++search_index;
          continue;
        }

        const uint32_t curr_id = prune->ids[search_index];

        if(curr_id == id_to_search)
        {
          // Remove duplicate id.
          memmove(&prune->ids[search_index], &prune->ids[search_index+1], (prune->size-search_index-1)*sizeof(prune->ids));
          --prune->size;
        }

        ++search_index;
      }

      ++prune_index;
    } // remove duplicate ids
  }
}


void
prune_free(Prune *prune)
{
  assert(prune);

  delete[] prune->ids;
}


} // ns
} // ns
