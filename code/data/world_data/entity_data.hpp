
/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/

#ifndef ENTITY_DATA_INCLUDED_65CE2E8B_AE72_4BCF_80FE_7547FA17BE75
#define ENTITY_DATA_INCLUDED_65CE2E8B_AE72_4BCF_80FE_7547FA17BE75


#include <utilities/generic_id.hpp>
#include <utilities/memory_pool.hpp>
#include <stddef.h>
#include <stdint.h>


namespace World_data {


namespace Entity_component {
  enum ENUM {
    has_physics = 1 << 0,
  };
} // ns



struct Entity_data
{
  // Data Key
  util::generic_id *data_key = nullptr;

  // Properties
  char *property_name = nullptr;
  uint32_t *property_tag = nullptr;
  uint32_t *property_components = nullptr;
  uintptr_t *property_user_data = nullptr;

  // Size information
  size_t size = 0;
  const size_t capacity = 0;

  // Memory chunk
  const util::memory_chunk memory = util::memory_chunk();

}; // struct


void
entity_data_init(Entity_data *data, const size_t size_hint);


void
entity_data_free(Entity_data *data);


size_t
entity_data_get_size(const Entity_data *data);


size_t
entity_data_get_capacity(const Entity_data *data);


void
data_lock(Entity_data *data);


void
data_unlock(Entity_data *data);


bool
entity_data_push_back(Entity_data *data, const util::generic_id key, size_t *out_index = nullptr);


bool
entity_data_erase(Entity_data *data, const util::generic_id key);


bool
entity_data_exists(const Entity_data *data, const util::generic_id key, size_t *out_index = nullptr);


bool
entity_data_get_property_name(const Entity_data *data, const util::generic_id key, const char **value);


bool
entity_data_set_property_name(Entity_data *data,  const util::generic_id key, const char *value);


bool
entity_data_get_property_tag(const Entity_data *data, const util::generic_id key, uint32_t *value);


bool
entity_data_set_property_tag(Entity_data *data,  const util::generic_id key, const uint32_t value);


bool
entity_data_get_property_components(const Entity_data *data, const util::generic_id key, uint32_t *value);


bool
entity_data_set_property_components(Entity_data *data,  const util::generic_id key, const uint32_t value);


bool
entity_data_get_property_user_data(const Entity_data *data, const util::generic_id key, uintptr_t *value);


bool
entity_data_set_property_user_data(Entity_data *data,  const util::generic_id key, const uintptr_t value);


} // ns


#endif // inc guard
