#ifndef BUFFER_DATA_INCLUDED_335AA4BA_133F_433B_8505_0A24006918EC
#define BUFFER_DATA_INCLUDED_335AA4BA_133F_433B_8505_0A24006918EC


#include "../common/common.hpp"


namespace op {


struct buffer_data
{
  // -- Bufffer Data -- //

  uint8_t       *buffer_data          = nullptr;
  size_t        buffer_size           = 0;
  size_t        buffer_capacity       = 0;
  size_t        buffer_read_pointer   = 0;

  // -- Callbacks -- //

  allocCallback     alloc_cb;
  reallocCallback   realloc_cb;
  freeCallback      destroy_cb;
  uintptr_t         user_data;

  // -- Methods -- //

  bool
  has_data() const;

  bool
  has_capacity(const size_t sizeof_data) const;

  void
  reset();

  void
  initial_reserve(const size_t size_to_initialize);

  void
  destroy_buffer();

  void
  write_data(void *data, size_t sizeof_data);

  void*
  read_data();
};


} // ns


#endif // inc guard
