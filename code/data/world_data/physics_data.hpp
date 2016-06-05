

/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/


#ifndef PHYSICS_DATA_INCLUDED_3EBA9CC0_AC2C_42D2_9B15_E9F1528BE11E
#define PHYSICS_DATA_INCLUDED_3EBA9CC0_AC2C_42D2_9B15_E9F1528BE11E


#include <utilities/generic_id.hpp>
#include <utilities/memory_pool.hpp>
#include <stddef.h>
#include <math/transform/transform.hpp>
#include <math/geometry/aabb.hpp>
#include <stdint.h>


namespace World_data {



/*!
  \brief Auto-generated structure for Physics_data
*/
struct Physics_data
{
  // Data Key
  util::generic_id *entity_id = nullptr;

  // Properties
  math::transform *property_transform = nullptr;
  math::aabb *property_aabb_collider = nullptr;
  uint64_t *property_collision_id = nullptr;

  // Size information
  size_t size = 0;
  const size_t capacity = 0;

  // Memory chunk
  const util::memory_chunk memory = util::memory_chunk();

}; // struct


/*!
  \brief Initialize the Physics_data structure, this is will allocate the memory for the keys and properties. Function will take a lock.
  \param data This structure to initialize.
  \param size_hint This helps the init function allocate the correct memory.
*/
void
physics_data_init(Physics_data *data, const size_t size_hint);


/*!
  \brief Not currently implimented, but will return the memory. Function will take a lock.
  \param data The data to free.
*/
void
physics_data_free(Physics_data *data);


/*!
  \brief Gets the number of items in the container.
  \param data The structure which to find the size.
*/
size_t
physics_data_get_size(const Physics_data *data);


/*!
  \brief Gets the capacity before new memory is needed.
  \param data The structure which to find the capacity.
*/
size_t
physics_data_get_capacity(const Physics_data *data);


/*!
  \brief Locks the data, any thing else requiring a lock will have to wait (not implimented).
  \param data The container to lock.
*/
void
data_lock(Physics_data *data);


/*!
  \brief Unlocks the data, any pending locks can now proceed (not implimented).
  \param The container to unlock
*/
void
data_unlock(Physics_data *data);


/*!
  \brief Push back a new element, increases the size by 1. (Does not take a lock).
  \param data The container which to push back.
  \param key The id to use as a key.
  \param out_index Optional - this will return the inserted location in the container.
  \return Returns true if it was successful.
*/
bool
physics_data_push_back(Physics_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Erase an item from the container, decreases size by 1. (Does not take a lock).
  \param data The container to erase from.
  \param key The key which is used to remove the data.
  \return Returns true if it was successful.
*/
bool
physics_data_erase(Physics_data *data, const util::generic_id key);


/*!
  \brief Checks to see if a key exists in the container.
  \param data The container to search.
  \param key The key to search for.
  \param out_index Optional - this will return the location of the key.
  \return Returns true if a key was found.
*/
bool
physics_data_exists(const Physics_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Getter for property_transform.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
physics_data_get_property_transform(const Physics_data *data, const util::generic_id key, math::transform *value);


/*!
  \brief Setter for property_transform.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
physics_data_set_property_transform(Physics_data *data,  const util::generic_id key, const math::transform value);


/*!
  \brief Getter for property_aabb_collider.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
physics_data_get_property_aabb_collider(const Physics_data *data, const util::generic_id key, math::aabb *value);


/*!
  \brief Setter for property_aabb_collider.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
physics_data_set_property_aabb_collider(Physics_data *data,  const util::generic_id key, const math::aabb value);


/*!
  \brief Getter for property_collision_id.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
physics_data_get_property_collision_id(const Physics_data *data, const util::generic_id key, uint64_t *value);


/*!
  \brief Setter for property_collision_id.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
physics_data_set_property_collision_id(Physics_data *data,  const util::generic_id key, const uint64_t value);


} // ns


#endif // inc guard
