#ifndef MATERIAL_RENDERER_INCLUDED_4307B221_E5ED_4329_B7DC_6AE309DCF1AF
#define MATERIAL_RENDERER_INCLUDED_4307B221_E5ED_4329_B7DC_6AE309DCF1AF


#include <core/resources/resources_fwd.hpp>
#include <memory>


namespace Core {


class Material_renderer final
{
public:

  explicit                Material_renderer();
                          ~Material_renderer();
  
  void                    set_material(const Material &material);
  Material                get_material() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl>   m_impl;

}; // class


} // ns


#endif // inc guard