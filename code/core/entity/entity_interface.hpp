#ifndef ENTITY_INTERFACE_INCLUDED_4F2BF5DD_F90C_45AA_9885_FD91FD5CD5A5
#define ENTITY_INTERFACE_INCLUDED_4F2BF5DD_F90C_45AA_9885_FD91FD5CD5A5


#include <core/entity_id.hpp>
#include <core/transform/transform.hpp>
#include <core/physics/collider.hpp>
#include "detail/entity_detail.hpp"
#include <data/world_data/world_pools.hpp>
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
  
protected: // This should be private!

  ENTITY_MEMBERS

}; // class


} // ns


#endif // inc guard