#ifndef ENTITY_INTERFACE_INCLUDED_4F2BF5DD_F90C_45AA_9885_FD91FD5CD5A5
#define ENTITY_INTERFACE_INCLUDED_4F2BF5DD_F90C_45AA_9885_FD91FD5CD5A5


#include <utilities/generic_id.hpp>
#include <core/common/core_fwd.hpp>
#include <data/world_data/world_data_fwd.hpp>
#include <stdint.h>
#include <memory>


namespace Core {
namespace Entity_detail {



util::generic_id              get_id(const util::generic_id this_id, World_data::World *world);
bool                          is_valid(const util::generic_id this_id, World_data::World *world);

void                          destroy(const util::generic_id this_id, World_data::World *world);

void                          set_user_data(const util::generic_id this_id, World_data::World *world, const uintptr_t user_data);
uintptr_t                     get_user_data(const util::generic_id this_id, World_data::World *world);

uint32_t                      get_tags(const util::generic_id this_id, World_data::World *world);
bool                          has_tag(const util::generic_id this_id, World_data::World *world, const uint32_t tag_id);
void                          set_tags(const util::generic_id this_id, World_data::World *world, const uint32_t set_tags);
void                          add_tag(const util::generic_id this_id, World_data::World *world, const uint32_t add_tag);
void                          remove_tag(const util::generic_id this_id, World_data::World *world, const uint32_t tag);

void                          set_name(const util::generic_id this_id, World_data::World *world, const char* set_name);
const char*                   get_name(const util::generic_id this_id, World_data::World *world);

void                          set_transform(const util::generic_id this_id, World_data::World *world, const Transform &transform);
Transform                     get_transform(const util::generic_id this_id, World_data::World *world);

void                          set_renderer(const util::generic_id this_id, World_data::World *world, const Core::Renderer &renderer);
Core::Renderer                get_renderer(const util::generic_id this_id, World_data::World *world);

void                          set_renderer_material(const util::generic_id this_id, World_data::World *world, const util::generic_id material_id, const util::generic_id model_id);
void                          get_renderer_material(const util::generic_id this_id, World_data::World *world, util::generic_id *out_material_id, util::generic_id *out_model_id);

void                          set_collider(const util::generic_id this_id, World_data::World *world, const Core::Collider &collider);
Core::Collider                get_collider(const util::generic_id this_id, World_data::World *world);

void                          set_rigidbody(const util::generic_id this_id, World_data::World *world, const Core::Rigidbody &rigidbody);
Core::Rigidbody               get_rigidbody(const util::generic_id this_id, World_data::World *world);
 
void                          set_rigidbody_properties(const util::generic_id this_id, World_data::World *world, const Core::Rigidbody_properties props);
Core::Rigidbody_properties    get_rigidbody_properties(const util::generic_id this_id, World_data::World *world);
  

} // ns
} // ns


#endif // inc guard