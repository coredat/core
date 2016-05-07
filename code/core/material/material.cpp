#include "material.hpp"


namespace Core {


struct Material::Impl
{
};


Material::Material(const char *name)
: m_impl(new Impl())
{

}


Material::~Material()
{
}


Material::Material(const Material &mat)
{
}


Material::Material(Material &&mat)
{
}

Material&
Material::operator=(const Material &mat)
{
  return *this;
}


Material&
Material::operator=(Material &&mat)
{
  return *this;
}


void
Material::set_shader(const Material_shader &shader)
{
}


void
Material::set_map_01(const Texture &texture)
{
}


void
Material::set_map_02(const Texture &texture)
{
}


void
Material::set_map_03(const Texture &texture)
{
}


const char *
Material::get_name() const
{
  return "";
}



} // ns