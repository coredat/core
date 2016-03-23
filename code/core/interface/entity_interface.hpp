#ifndef ENTITY_INTERFACE_INCLUDED_4F2BF5DD_F90C_45AA_9885_FD91FD5CD5A5
#define ENTITY_INTERFACE_INCLUDED_4F2BF5DD_F90C_45AA_9885_FD91FD5CD5A5


#include "component.hpp"
#include <core/entity_id.hpp>
#include <core/transform/transform.hpp>
#include <core/physics/collider.hpp>
#include "detail/entity_detail.hpp"
#include <data/world_data/world_pools.hpp>
#include <data/world_data/logic_pool.hpp>
#include <stdint.h>


namespace Core {


class Entity_interface
{
public:

  Core::Entity_id               get_id() const;
  bool                          is_valid() const;
  
  uint32_t                      get_tags() const;
  bool                          has_tag(const uint32_t tag_id) const;
  void                          set_tags(const uint32_t set_tags);
  void                          add_tag(const uint32_t add_tag);
  void                          remove_tag(const uint32_t tag);
  
  void                          set_name(const char* set_name);
  const char*                   get_name() const;
  
//  void                          set_parent(const Core::Entity_id id);
//  Entity                        get_parent() const;
//  
//  uint32_t                      get_number_of_children() const;
//  Entity                        get_child(const uint32_t index) const;

  void                          send_event(const uint32_t id, const void *data, const uint32_t size_of_data);

  void                          set_transform(const Transform &transform);
  Transform                     get_transform() const;

  void                          set_material_id(const uint32_t id);
  uint32_t                      get_material_id() const;
  
  void                          set_model_id(const uint32_t id);
  uint32_t                      get_model_id() const;
  
  void                          set_collider(const Collider &collider);
  Collider                      get_collider() const;
  
  
  template<typename T>
  T*
  add_component()
  {
    const auto free_slot = World_data::logic_pool_get_slot(m_world_data->logic_pool, get_id());
    
    if(free_slot)
    {
      auto comp = new(free_slot) T();

      auto base = reinterpret_cast<Core::Component*>(free_slot);
      base->m_world_data = m_world_data;
      base->m_entity_id  = this->get_id();
      
      return comp;
    }
    
    assert(false); // Soemthing went wrong!
    return nullptr;
  }
  
  
  template<typename T>
  T* get_component(const uint32_t rtti_id)
  {
    const uint32_t count = m_world_data->logic_pool->size;
    
    for(uint32_t i = 0; i < count; ++i)
    {
      if(m_world_data->logic_pool->entity_id[i] == get_id())
      {
        // Get script.
        void *comp = &m_world_data->logic_pool->object_store[i * LOGIC_POOL_SIZE_MAX_SCRIPT_SIZE];
      
        if(reinterpret_cast<Core::Component*>(comp)->get_rtti() == rtti_id)
        {
          return reinterpret_cast<T*>(comp);
        }
      }
    }
    
    return nullptr;
  }
  
  uint32_t get_number_of_components() const;
  
protected: // This should be private!

  ENTITY_MEMBERS

}; // class


} // ns


#endif // inc guard