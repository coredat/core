#include <core/renderer/material_renderer.hpp>
#include <core/renderer/renderer.hpp>
#include <core/resources/material.hpp>
#include <core/model/model.hpp>


namespace Core {


Material_renderer::Material_renderer(const uint32_t mat_id,
                                     const uint32_t model_id)
: m_material_id(mat_id)
, m_model_id(model_id)
{
}


Material_renderer::Material_renderer(const Material &material,
                                     const Model &model)
: Material_renderer(material.get_id(), model.get_id())
{
}


Material_renderer::Material_renderer()
: Material_renderer(0, 0)
{
}


Material_renderer::~Material_renderer()
{
}


void
Material_renderer::set_material(const Core::Material &material)
{
  m_material_id = material.get_id();
}


Material
Material_renderer::get_material() const
{
  return Material(m_material_id);
}


void
Material_renderer::set_model(const Core::Model &model)
{
  m_model_id = model.get_id();
}


Model
Material_renderer::get_model() const
{
  return Model(m_model_id);
}


uint32_t
Material_renderer::get_material_id() const
{
  return m_material_id;
}


uint32_t
Material_renderer::get_model_id() const
{
  return m_model_id;
}


bool
Material_renderer::is_valid() const
{
  return (m_material_id != 0 && m_model_id != 0);
}


//Material_renderer::operator Renderer() const
//{
//  return Renderer(Core::Renderer_type::material, m_material_id, m_model_id);
//}


Material_renderer::operator bool() const
{
  return is_valid();
}


} // ns
