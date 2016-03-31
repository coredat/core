#include <core/model/mesh.hpp>
#include <math/geometry/geometry_types.hpp>
#include <stdint.h>
#include <assert.h>


namespace Core {


struct Mesh::Impl
{
  uint32_t mesh_id = 0;
};


Mesh::Mesh()
: m_impl(new Impl)
{
}


Mesh::~Mesh()
{
}


Mesh::Mesh(const Mesh &other)
: m_impl(new Impl(*other.m_impl))
{
}


Mesh::Mesh(Mesh &&other)
: m_impl(new Impl(*other.m_impl))
{
}


math::aabb
Mesh::get_aabb() const
{
  assert(false);
  return math::aabb();
}


uint32_t
Mesh::get_number_of_vertices() const
{
  assert(false);
  return 0;
}


bool
Mesh::is_valid() const
{
  assert(false);
  return m_impl->mesh_id > 0;
}


} // ns