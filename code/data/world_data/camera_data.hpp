

/*
  Warning:
  This file is auto_generated any changes here may be overwritten.
*/


#ifndef CAMERA_DATA_INCLUDED_FD80B963_605C_46DF_BF0E_B5BB2627CFBB
#define CAMERA_DATA_INCLUDED_FD80B963_605C_46DF_BF0E_B5BB2627CFBB


#include <utilities/generic_id.hpp>
#include <utilities/memory_pool.hpp>
#include <stddef.h>
#include <stdint.h>
#include <systems/camera/camera_properties.hpp>


namespace World_data {



/*!
  \brief Auto-generated structure for Camera_data
*/
struct Camera_data
{
  // Data Key
  util::generic_id *camera_id = nullptr;

  // Properties
  util::generic_id *property_entity_id = nullptr;
  uint32_t *property_priority = nullptr;
  util::generic_id *property_texture_id = nullptr;
  Camera::Camera_properties *property_camera = nullptr;

  // Size information
  size_t size = 0;
  const size_t capacity = 0;

  // Memory chunk
  const util::memory_chunk memory = util::memory_chunk();

}; // struct


/*!
  \brief Initialize the Camera_data structure, this is will allocate the memory for the keys and properties. Function will take a lock.
  \param data This structure to initialize.
  \param size_hint This helps the init function allocate the correct memory.
*/
void
camera_data_init(Camera_data *data, const size_t size_hint);


/*!
  \brief Not currently implimented, but will return the memory. Function will take a lock.
  \param data The data to free.
*/
void
camera_data_free(Camera_data *data);


/*!
  \brief Gets the number of items in the container.
  \param data The structure which to find the size.
*/
size_t
camera_data_get_size(const Camera_data *data);


/*!
  \brief Gets the capacity before new memory is needed.
  \param data The structure which to find the capacity.
*/
size_t
camera_data_get_capacity(const Camera_data *data);


/*!
  \brief Locks the data, any thing else requiring a lock will have to wait (not implimented).
  \param data The container to lock.
*/
void
data_lock(const Camera_data *data);


/*!
  \brief Unlocks the data, any pending locks can now proceed (not implimented).
  \param The container to unlock
*/
void
data_unlock(const Camera_data *data);


/*!
  \brief Push back a new element, increases the size by 1. (Does not take a lock).
  \param data The container which to push back.
  \param key The id to use as a key.
  \param out_index Optional - this will return the inserted location in the container.
  \return Returns true if it was successful.
*/
bool
camera_data_push_back(Camera_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Erase an item from the container, decreases size by 1. (Does not take a lock).
  \param data The container to erase from.
  \param key The key which is used to remove the data.
  \return Returns true if it was successful.
*/
bool
camera_data_erase(Camera_data *data, const util::generic_id key);


/*!
  \brief Inserts a new element shuffling down any data that exists in the array. (Does not take a lock).
  \param data The container to insert into.
  \param key The key which is used to remove the data.
  \param insert_point Where to insert. If bigger then the size it will push back.
  \return Returns true if it was successful.
*/
bool
camera_data_insert(Camera_data *data, const util::generic_id key, const size_t insert_index);


/*!
  \brief Checks to see if a key exists in the container.
  \param data The container to search.
  \param key The key to search for.
  \param out_index Optional - this will return the location of the key.
  \return Returns true if a key was found.
*/
bool
camera_data_exists(const Camera_data *data, const util::generic_id key, size_t *out_index = nullptr);


/*!
  \brief Getter for property_entity_id.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
camera_data_get_property_entity_id(const Camera_data *data, const util::generic_id key, util::generic_id *value);


/*!
  \brief Setter for property_entity_id.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
camera_data_set_property_entity_id(Camera_data *data,  const util::generic_id key, const util::generic_id value);


/*!
  \brief Getter for property_priority.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
camera_data_get_property_priority(const Camera_data *data, const util::generic_id key, uint32_t *value);


/*!
  \brief Setter for property_priority.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
camera_data_set_property_priority(Camera_data *data,  const util::generic_id key, const uint32_t value);


/*!
  \brief Getter for property_texture_id.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
camera_data_get_property_texture_id(const Camera_data *data, const util::generic_id key, util::generic_id *value);


/*!
  \brief Setter for property_texture_id.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
camera_data_set_property_texture_id(Camera_data *data,  const util::generic_id key, const util::generic_id value);


/*!
  \brief Getter for property_camera.
  \param data The container to get information from.
  \param key The key to search for.
  \param value The output value, which will be set if the key is found.
  \return Returns true if the data was found.
*/
bool
camera_data_get_property_camera(const Camera_data *data, const util::generic_id key, Camera::Camera_properties *value);


/*!
  \brief Setter for property_camera.
  \param data The container to update.
  \param key The key to search for.
  \param value The new value of the data.
  \return Returns true if the data was set.
*/
bool
camera_data_set_property_camera(Camera_data *data,  const util::generic_id key, const Camera::Camera_properties value);


} // ns


#endif // inc guard
