#ifndef MATERIAL_RENDERER_INCLUDED_4307B221_E5ED_4329_B7DC_6AE309DCF1AF
#define MATERIAL_RENDERER_INCLUDED_4307B221_E5ED_4329_B7DC_6AE309DCF1AF


#include <memory>


namespace Core {


class Material_renderer
{
public:

  explicit                Material_renderer();
                          ~Material_renderer();
  
  void                    set_material(const int mesh);
  int                     get_material() const;

  void                    set_mesh(const int mesh);
  int                     get_mesh() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl>   m_impl;

}; // class


} // ns


#endif // inc guard