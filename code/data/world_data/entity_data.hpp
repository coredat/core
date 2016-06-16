

/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/


#ifndef ENTITY_DATA_INCLUDED_2FBA7BD8_346D_4D6A_831D_FBF44ECCD8BD
#define ENTITY_DATA_INCLUDED_2FBA7BD8_346D_4D6A_831D_FBF44ECCD8BD


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




/*!
  \brief Auto-generated structure for Entity_data
*/
struct Entity_data
{
  // Data Key
  util::generic_id *entity_id = nullptr;

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


/*!
  \brief Initialize the Entity_data structure, this is will allocate the memory for the keys and properties. Function will take a lock.
  \param data This structure to initialize.
  \param size_hint This helps the init function allocate the correct memory.
*/
void
entity_data_init(Entity_data *data, const size_t size_hint);


/*!
  \brief Not currently implimented, but will return the memory. Function will take a lock.
  \param data The data to free.
*/
void
entity_data_free(Entity_data *data);


/*!
  \brief Gets the number of items in the container.
  \param data The structure which to find the size.
*/
size_t
entity_data_get_size(const Entity_data *data);


/*!
  \brief Gets the capacity before new memory is needed.
  \param data The structure which to find the capacity.
*/
size_t
entity_data_get_capacity(const Entity_data *data);


/*!
  \brief Locks the data, any thing else requiring a lock will have to wait (not implimented).
  \param data The container to lock.
*/
void
data_lock(Entity_data *data);


/*!
  \brief Unlocks the data, any pending locks can now proceed (not implimented).
  \param The container to unlock
*/
void
data_unlock(Entity_data *data);


/*!
  \brief Push back a new element, increases the size by 1. (Does not take a lock).
  \param data The container which to push back.
  \param key The id to use as a key.
  \param out_index Optional - this will return the inserted location in the container.
  \return Returns true if it was successful.
*/
bool
entity_data_push_back(Entity_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Erase an item from the container, decreases size by 1. (Does not take a lock).
  \param data The container to erase from.
  \param key The key which is used to remove the data.
  \return Returns true if it was successful.
*/
bool
entity_data_erase(Entity_data *data, const util::generic_id key);


/*!
  \brief Checks to see if a key exists in the container.
  \param data The container to search.
  \param key The key to search for.
  \param out_index Optional - this will return the location of the key.
  \return Returns true if a key was found.
*/
bool
entity_data_exists(const Entity_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Getter for property_name.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
entity_data_get_property_name(const Entity_data *data, const util::generic_id key, const char **value);


/*!
  \brief Setter for property_name.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
entity_data_set_property_name(Entity_data *data,  const util::generic_id key, const char *value);


/*!
  \brief Getter for property_tag.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
entity_data_get_property_tag(const Entity_data *data, const util::generic_id key, uint32_t *value);


/*!
  \brief Setter for property_tag.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
entity_data_set_property_tag(Entity_data *data,  const util::generic_id key, const uint32_t value);


/*!
  \brief Getter for property_components.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
entity_data_get_property_components(const Entity_data *data, const util::generic_id key, uint32_t *value);


/*!
  \brief Setter for property_components.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
entity_data_set_property_components(Entity_data *data,  const util::generic_id key, const uint32_t value);


/*!
  \brief Getter for property_user_data.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
entity_data_get_property_user_data(const Entity_data *data, const util::generic_id key, uintptr_t *value);


/*!
  \brief Setter for property_user_data.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
entity_data_set_property_user_data(Entity_data *data,  const util::generic_id key, const uintptr_t value);


} // ns


#endif // inc guard
