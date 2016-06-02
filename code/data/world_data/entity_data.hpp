#ifndef ENTITY_DATA_INCLUDED_18E6DF4A_1B5A_44CE_A75F_FBC3054389B2
#define ENTITY_DATA_INCLUDED_18E6DF4A_1B5A_44CE_A75F_FBC3054389B2


#include <utilities/generic_id.hpp>
#include <utilities/memory_pool.hpp>
#include <utilities/bits.hpp>
#include <utilities/generic_id.hpp>
#include <stdint.h>
#include <stddef.h>
#include <utilities/memory_pool.hpp>

namespace World_data {


namespace Entity_component {

enum ENUM
{
  has_physics = BIT(0),
};

} // ns



struct Entity_data
{
  // Index keys;
  util::generic_id *data_key = nullptr;
  // Properties;
  char *property_name = nullptr;
  uint32_t *property_tag = nullptr;
  uint32_t *property_components = nullptr;
  uintptr_t *property_user_data = nullptr;
  // Size and capacity;
  size_t size = 0;
  const size_t capacity = 0;
  //Memory Pool;
  const util::memory_chunk memory = util::memory_chunk();
};


// Insert a new entry to the resource
bool
entity_data_push_back(Entity_data *data, const util::generic_id key);


// Remove an entry from the resource
bool
entity_data_erase(Entity_data *data, const util::generic_id key);


// Checks to see if a entry exists for the key
bool
entity_data_exists(const Entity_data *data, const util::generic_id key, size_t *out_index = nullptr);


// Is data valid.
bool
entity_data_is_valid(const Entity_data *data);


// Locks the resources
void
data_lock(Entity_data *data);


// Unlocks the resource
void
data_unlock(Entity_data *data);


bool
entity_data_get_name(const Entity_data *data, const util::generic_id key, char *out_value);


bool
entity_data_set_name(Entity_data *data, const util::generic_id key, const char *value);


bool
entity_data_get_tag(const Entity_data *data, const util::generic_id key, uint32_t *out_value);


bool
entity_data_set_tag(Entity_data *data, const util::generic_id key, const uint32_t *value);


bool
entity_data_get_components(const Entity_data *data, const util::generic_id key, uint32_t *out_value);


bool
entity_data_set_components(Entity_data *data, const util::generic_id key, const uint32_t *value);


bool
entity_data_get_user_data(const Entity_data *data, const util::generic_id key, uintptr_t *out_value);


bool
entity_data_set_user_data(Entity_data *data, const util::generic_id key, const uintptr_t *value);


// Current size of the data store.
size_t
entity_data_get_size(const Entity_data *data);


// Current capacity of the data store.
size_t
entity_data_get_capacity(const Entity_data *data);


// Initilizes the data resource.
void
entity_data_init(Entity_data *data, const size_t size_hint);


// Frees the resources.
void
entity_data_free(Entity_data *data);


} // ns


#endif // inc guard