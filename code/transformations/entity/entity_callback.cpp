#include "entity_callback.hpp"
#include <data/world/entity_data.hpp>
#include <assert.h>


namespace Entity_detail {


void
set_entity_collision_callback(const util::generic_id this_id,
                              Data::Entity_data *entity_data,
                              const uintptr_t callback_ptr,
                              const uintptr_t user_data)
{
  // Param
  assert(this_id);
  assert(entity_data);

  // Add / update callback
  {
    Data::data_lock(entity_data);
    
    if(Data::entity_exists(entity_data, this_id))
    {
      Callback_collision callback
      {
        user_data,
        callback_ptr
      };
      
      Data::entity_set_collision_callback(entity_data, this_id, &callback);
    }
    
    Data::data_unlock(entity_data);
  }
}


} // ns