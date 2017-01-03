#include "context_data.hpp"
#include "../common/common.hpp"

#include "../api/ogl/descs/descs.hpp"


namespace op {


void
context_data::initial_reserve()
{
  auto resource_alloc = [](const uint32_t init_count,
                           const uint32_t init_bytes,
                           const allocation_cb alloc_cb,
                           const uintptr_t user_data,
                           auto out_resource_ptr,
                           size_t *out_set_capacity,
                           size_t *out_set_size)
  {
    // -- Param Check -- //
    assert(alloc_cb);
    assert(init_count);
    assert(out_set_capacity);
    assert(out_set_size);
    assert(out_resource_ptr);

    // -- Create Resource -- //
    *out_resource_ptr = (decltype(+*out_resource_ptr))alloc_cb(init_bytes, user_data);

    if(out_resource_ptr)
    {
      *out_set_capacity = init_count;
      *out_set_size = 0;
    }
  };

  // -- Inital allocation for space -- //

  // Shader Descs //
  {
    log("Shader desc alloc");
    constexpr uint32_t default_shader_count = 16;
    resource_alloc(default_shader_count,
                   default_shader_count * sizeof(API::shader_desc),
                   alloc_cb,
                   user_data,
                   &shader_descs,
                   &shader_desc_capacity,
                   &shader_desc_size);
  }

  // Shader Uniform Descs //
  {
    log("Shader Uniform desc alloc");
    constexpr uint32_t default_uniform_count = 128;
    resource_alloc(default_uniform_count,
                   default_uniform_count * sizeof(API::uniform_desc),
                   alloc_cb,
                   user_data,
                   &uniform_descs,
                   &uniform_desc_capacity,
                   &uniform_desc_size);
  }

  // Shader Data Descs //
  {
    log("Shader data desc alloc");
    constexpr uint32_t default_shader_data_count = 64;
    resource_alloc(default_shader_data_count,
                   default_shader_data_count * sizeof(API::shader_data_desc),
                   alloc_cb,
                   user_data,
                   &shader_data_descs,
                   &shader_data_desc_capacity,
                   &shader_data_desc_size);
  }

  // Target Descs //
  {
    log("Target desc alloc");
    constexpr uint32_t default_target_count = 16;
    resource_alloc(default_target_count,
                   default_target_count * sizeof(API::target_desc),
                   alloc_cb,
                   user_data,
                   &target_descs,
                   &target_desc_capacity,
                   &target_desc_size);
  }

  // Vertex Format //
  {
    log("Vertex format desc alloc");
    constexpr uint32_t default_vertex_format_count = 16;
    resource_alloc(default_vertex_format_count,
                   default_vertex_format_count * sizeof(API::vertex_format_desc),
                   alloc_cb,
                   user_data,
                   &vertex_format_descs,
                   &vertex_format_desc_capacity,
                   &vertex_format_desc_size);
  }

  // Geometry //
  {
    log("Geometry desc alloc");
    constexpr uint32_t default_geometry_count = 128;
    resource_alloc(default_geometry_count,
                   default_geometry_count * sizeof(API::geometry_desc),
                   alloc_cb,
                   user_data,
                   &geometry_descs,
                   &geometry_desc_capacity,
                   &geometry_desc_size);
  }

  // Index //
  {
    log("Index desc alloc");
    constexpr uint32_t default_index_count = 128;
    resource_alloc(default_index_count,
                   default_index_count * sizeof(API::index_internal_desc),
                   alloc_cb,
                   user_data,
                   &index_descs,
                   &index_desc_capacity,
                   &index_desc_size);
  }

  // Rasterizer //
  {
    log("Rasterizer desc alloc");
    constexpr uint32_t default_rasterizer_count = 128;
    resource_alloc(default_rasterizer_count,
                   default_rasterizer_count * sizeof(API::rasterizer_desc),
                   alloc_cb,
                   user_data,
                   &rasterizer_descs,
                   &rasterizer_desc_capacity,
                   &rasterizer_desc_size);
  }

  // Texture //
  {
    log("Texture desc alloc");
    constexpr uint32_t default_texture_count = 128;
    resource_alloc(default_texture_count,
                   default_texture_count * sizeof(API::texture_desc),
                   alloc_cb,
                   user_data,
                   &texture_descs,
                   &texture_desc_capacity,
                   &texture_desc_size);
  }

  // Texture Filter //
  {
    log("Texture Filter desc alloc");
    constexpr uint32_t default_texture_filtering_count = 8;
    resource_alloc(default_texture_filtering_count,
                   default_texture_filtering_count * sizeof(API::texture_filtering_internal_desc),
                   alloc_cb,
                   user_data,
                   &texture_filtering_descs,
                   &texture_filtering_desc_capacity,
                   &texture_filtering_desc_size);
  }

}


resource_id
context_data::add_shader()
{
  // -- Member Check -- //
  assert(shader_descs);
  assert(shader_desc_capacity);

  // -- Full -- //
  if(shader_desc_size == shader_desc_capacity)
  {
    return invalid_resource_id();
  }

  // -- Generate Resource -- //
  uint32_t instance = (uint32_t)shader_desc_size;
  shader_desc_size += 1;

  memset(
    &shader_descs[instance],
    0,
    sizeof(API::shader_desc)
  );

  return resource_id{resource_type::SHADER, instance};
}


resource_id
context_data::add_texture()
{
  // -- Member Check -- //
  assert(texture_descs);
  assert(texture_desc_capacity);

  // -- Full -- //
  if(texture_desc_size == texture_desc_capacity)
  {
    return invalid_resource_id();
  }

  // -- Generate Resource -- //
  uint32_t instance = (uint32_t)texture_desc_size;
  texture_desc_size += 1;

  memset(
    &texture_descs[instance],
    0,
    sizeof(API::texture_desc)
  );

  return resource_id{resource_type::TEXTURE, instance};
}


resource_id
context_data::add_texture_filter()
{
  // -- Member Check -- //
  assert(texture_filtering_descs);
  assert(texture_filtering_desc_capacity);

  // -- Full -- //
  if(texture_filtering_desc_size == texture_filtering_desc_capacity)
  {
    return invalid_resource_id();
  }

  // -- Generate Resource -- //
  uint32_t instance = (uint32_t)texture_filtering_desc_size;
  texture_desc_size += 1;

  memset(
    &texture_filtering_descs[instance],
    0,
    sizeof(API::texture_desc)
  );

  return resource_id{resource_type::TEXTURE_FILTER, instance};
}


resource_id
context_data::add_shader_data()
{
  // -- Member Check -- //
  assert(shader_data_descs);
  assert(shader_data_desc_capacity);

  // -- Full -- //
  if(shader_data_desc_size == shader_data_desc_capacity)
  {
    return invalid_resource_id();
  }

  // -- Generate Resource -- //
  uint32_t instance = (uint32_t)shader_data_desc_size;
  shader_data_desc_size += 1;

  memset(
    &shader_data_descs[instance],
    0,
    sizeof(API::shader_data_desc)
  );

  return resource_id{resource_type::SHADER_DATA, instance};
}


resource_id
context_data::add_target()
{
  // -- Member Check -- //
  assert(target_descs);
  assert(target_desc_capacity);

  // -- Full -- //
  if(target_desc_size >= target_desc_capacity)
  {
    return invalid_resource_id();
  }

  // -- Generate Resource -- //
  uint32_t instance = (uint32_t)target_desc_size;
  target_desc_size += 1;

  memset(
    &target_descs[instance],
    0,
    sizeof(API::target_desc)
  );

  return resource_id{resource_type::TARGET, instance};
}


resource_id
context_data::add_geometry()
{
  // -- Member Check -- //
  assert(geometry_descs);
  assert(geometry_desc_capacity);

  // -- Full -- //
  if(geometry_desc_size >= geometry_desc_capacity)
  {
    return invalid_resource_id();
  }

  // -- Generate Resource -- //
  uint32_t instance = (uint32_t)geometry_desc_size;
  geometry_desc_size += 1;

  memset(
    &geometry_descs[instance],
    0,
    sizeof(API::geometry_desc)
  );

  return resource_id{resource_type::GEOMETRY, instance};
}


resource_id
context_data::add_index()
{
  // -- Member Check -- //
  assert(index_descs);
  assert(index_desc_capacity);

  // -- Full -- //
  if(index_desc_size >= index_desc_capacity)
  {
    return invalid_resource_id();
  }

  // -- Generate Resource -- //
  uint32_t instance = (uint32_t)index_desc_size;
  index_desc_size += 1;

  memset(
    &index_descs[instance],
    0,
    sizeof(API::index_internal_desc)
  );

  return resource_id{resource_type::INDEX, instance};
}


resource_id
context_data::add_vertex_format()
{
  // -- Member Check -- //
  assert(vertex_format_descs);
  assert(vertex_format_desc_capacity);

  // -- Full -- //
  if(vertex_format_desc_size >= vertex_format_desc_capacity)
  {
    return invalid_resource_id();
  }

  // -- Generate Resource -- //
  uint32_t instance = (uint32_t)vertex_format_desc_size;
  vertex_format_desc_size += 1;

  memset(
    &vertex_format_descs[instance],
    0,
    sizeof(API::vertex_format_desc)
  );

  return resource_id{resource_type::VERTEX_FORMAT, instance};
}


resource_id
context_data::add_rasterizer()
{
  // -- Member Check -- //
  assert(rasterizer_descs);
  assert(rasterizer_desc_capacity);

  // -- Full -- //
  if(rasterizer_desc_size >= rasterizer_desc_capacity)
  {
    return invalid_resource_id();
  }

  // -- Generate Resource -- //
  uint32_t instance = (uint32_t)rasterizer_desc_size;
  rasterizer_desc_size += 1;

  memset(
    &rasterizer_descs[instance],
    0,
    sizeof(API::rasterizer_desc)
  );

  return resource_id{resource_type::RASTERIZER, instance};
}


// --


void*
context_data::alloc(const size_t bytes)
{
  return alloc_cb(bytes, user_data);
}

void
context_data::free(void *ptr)
{
  destroy_cb(ptr, user_data);
}


// --


void
context_data::vlog(const char *output, va_list args)
{
  vprintf(output, args);
  printf("\n");
}

void
context_data::log(const char *output, ...)
{
  va_list args;
  va_start(args, output);
  vlog(output, args);
  va_end(args);
}


} // ns
