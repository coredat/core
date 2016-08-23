#ifndef MATERIAL_INCLUDED_0898BDC5_BA5E_4FB9_80D8_90978B814B3A
#define MATERIAL_INCLUDED_0898BDC5_BA5E_4FB9_80D8_90978B814B3A


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


/*!
  Materials are used to encapsulate shader and uniforms
  for the mesh renderer.
  --
  TODO:
  - Getters to get the attributes.
*/
namespace Core {


class Material final
{
private:

  friend class Material_renderer;

  explicit              Material(const util::generic_id mat_id);

public:

  explicit              Material();
  explicit              Material(const char *name);
                        ~Material();
  
                        Material(const Material &mat);
                        Material(Material &&mat);

  Material&             operator=(const Material &mat);
  Material&             operator=(Material &&mat);
  
  const char *          get_name() const;

  void                  set_shader(const Shader &shader);
  
  void                  set_map_01(const Texture &texture);
  Texture               get_map_01() const;
  
  void                  set_color(const Color color);
  Color                 get_color() const;
  
  bool                  exists() const;
                        operator bool() const;
  
//private:
  
  uint32_t              get_id() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};


} // ns


#endif // inc guard