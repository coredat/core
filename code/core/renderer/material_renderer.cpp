#include <core/renderer/material_renderer.hpp>
#include <core/resources/material.hpp>
#include <utilities/generic_id.hpp>


namespace Core {


struct Material_renderer::Impl
{
  util::generic_id entity_id   = util::generic_id_invalid();
  util::generic_id material_id = util::generic_id_invalid();
};


Material_renderer::Material_renderer()
: m_impl(new Impl)
{
}


Material_renderer::~Material_renderer()
{
}


void
Material_renderer::set_material(const Core::Material &material)
{
}


Material
Material_renderer::get_material() const
{
  return Material();
}


} // ns