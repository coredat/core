#include "prune.hpp"
#include "sweep.hpp"
#include <math/general/general.hpp>
#include <assert.h>

//#define CORE_USE_SCRATCH_ALLOC

#ifdef CORE_USE_SCRATCH_ALLOC
#include <data/global_data/memory_data.hpp>
#include <new>
#endif


#include <iostream> // die


namespace Physics {
namespace Broadphase {


void
prune_init(Prune *prune, const Sweep *sweep)
{
  assert(prune);

  const uint32_t capacity = sweep->size * 3;
  prune->capacity = capacity;
  prune->size = 0;

  #ifdef CORE_USE_SCRATCH_ALLOC
  const size_t bytes = sizeof(uint32_t) * capacity;
   
  prune->ids = new(memory::scratch_alloc(bytes)) uint32_t[capacity];
  #else
  prune->ids = new uint32_t[capacity];
  #endif
}


void
prune_calculate(Prune *prune, const Sweep *sweep)
{
  std::cout << "prune" << std::endl;

  // For each axis prune out objects that really can't be touching.
  constexpr uint32_t number_of_axis = 3;
  const Sweep_axis *axis[number_of_axis] = {sweep->x_axis, sweep->y_axis, sweep->z_axis};

  for(uint32_t a = 0; a < number_of_axis; ++a)
  {
    const Sweep_axis *curr_axis = axis[a];
  
    // Loop through each item and check if it collides with something on that axis.
    uint32_t obj_id = 0;
    bool potential_collision = false;

    while(obj_id < sweep->size)
    {
      const Sweep_axis *test_obj = &curr_axis[obj_id];

      uint32_t sweep_id = 0;
      potential_collision = false;

      while(sweep_id < sweep->size)
      {
        // Don't self test
        if(sweep_id == obj_id)
        {
          ++sweep_id;
          continue;
        }

        // Test if this object is overlapping
        const Sweep_axis *sweep_obj = &curr_axis[sweep_id];
        
        const float c1 = math::min(test_obj->center, sweep_obj->center);
        const float c2 = math::max(test_obj->center, sweep_obj->center);
        
        const float distance = math::abs(c2 - c1);
        const float combined_extent = test_obj->half_extent + sweep_obj->half_extent;

        // If distance < combined_extent
        // then they are colliding.
        if(distance < combined_extent)
        {
          // No point testing this object
          // anymore we know its potentially
          // colliding.
          
//          std::cout << "coll on axis: " << a << " id: " << obj_id << std::endl;
          
          potential_collision = true;
          break;
        }

        ++sweep_id;
      }

      // This can be pruned out.
      if(!potential_collision)
      {
        assert(prune->size < prune->capacity);
        prune->ids[prune->size] = obj_id;
        ++(prune->size);
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
          const uint32_t start_move = search_index + 1;
          const uint32_t end_move   = prune->size - 1 - search_index;
        
          memmove(&prune->ids[search_index],
                  &prune->ids[start_move],
                  end_move * sizeof(prune->ids[0]));
        
          // Remove duplicate id.
          --(prune->size);
          
          continue;
        }
        else
        {
          ++search_index;
        }
      }

      ++prune_index;
    } // remove duplicate ids
  }
}


void
prune_free(Prune *prune)
{
  assert(prune);
  
  #ifndef CORE_USE_SCRATCH_ALLOC
  delete[] prune->ids;
  #endif
}


} // ns
} // ns
