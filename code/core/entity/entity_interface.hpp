#ifndef ENTITY_INTERFACE_INCLUDED_4F2BF5DD_F90C_45AA_9885_FD91FD5CD5A5
#define ENTITY_INTERFACE_INCLUDED_4F2BF5DD_F90C_45AA_9885_FD91FD5CD5A5


#include <core/entity_id.hpp>
#include <core/transform/transform_fwd.hpp>
#include <core/physics/physics_fwd.hpp>
#include <core/model/model_fwd.hpp>
#include <data/world_data/world_pools.hpp>
#include <stdint.h>
#include <memory>


namespace Core {
namespace Entity_detail {



Core::Entity_id               get_id(const Core::Entity_id this_id, World_data::World *world);
bool                          is_valid(const Core::Entity_id this_id, World_data::World *world);

uint32_t                      get_tags(const Core::Entity_id this_id, World_data::World *world);
bool                          has_tag(const Core::Entity_id this_id, World_data::World *world, const uint32_t tag_id);
void                          set_tags(const Core::Entity_id this_id, World_data::World *world, const uint32_t set_tags);
void                          add_tag(const Core::Entity_id this_id, World_data::World *world, const uint32_t add_tag);
void                          remove_tag(const Core::Entity_id this_id, World_data::World *world, const uint32_t tag);

void                          set_name(const Core::Entity_id this_id, World_data::World *world, const char* set_name);
const char*                   get_name(const Core::Entity_id this_id, World_data::World *world);

void                          send_event(const Core::Entity_id this_id, World_data::World *world, const uint32_t id, const void *data, const uint32_t size_of_data);

void                          set_transform(const Core::Entity_id this_id, World_data::World *world, const Transform &transform);
Transform                     get_transform(const Core::Entity_id this_id, World_data::World *world);

void                          set_material_id(const Core::Entity_id this_id, World_data::World *world, const uint32_t id);
uint32_t                      get_material_id(const Core::Entity_id this_id, World_data::World *world);

void                          set_model(const Core::Entity_id this_id, World_data::World *world, const Core::Model &model);
Core::Model                   get_model(const Core::Entity_id this_id, World_data::World *world);

void                          set_collider(const Core::Entity_id this_id, World_data::World *world, const Core::Collider &collider);
Core::Collider                get_collider(const Core::Entity_id this_id, World_data::World *world);
  
  

} // ns
} // ns


#endif // inc guard