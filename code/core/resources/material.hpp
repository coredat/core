#ifndef MATERIAL_INCLUDED_0898BDC5_BA5E_4FB9_80D8_90978B814B3A
#define MATERIAL_INCLUDED_0898BDC5_BA5E_4FB9_80D8_90978B814B3A


#include <core/resources/resources_fwd.hpp>
#include <memory>
#include <stdint.h>


/*!
  Materials are used to encapsulate shader and uniforms
  for the mesh renderer.
*/
namespace Core {


class Material final
{
public:

  explicit              Material();
  explicit              Material(const char *name);
                        ~Material();
  
                        Material(const Material &mat);
                        Material(Material &&mat);

  Material&             operator=(const Material &mat);
  Material&             operator=(Material &&mat);

  void                  set_shader(const Shader &shader);
  void                  set_map_01(const Texture &texture);
  void                  set_map_02(const Texture &texture);
  void                  set_map_03(const Texture &texture);
  
  const char *          get_name() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};


} // ns


#endif // inc guard