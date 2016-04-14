#ifndef MATERIAL_SHADER_INCLUDED_AFE746EF_93EB_4121_A11B_ACBC4AFEA573
#define MATERIAL_SHADER_INCLUDED_AFE746EF_93EB_4121_A11B_ACBC4AFEA573


#include <memory>


namespace Core {


class Material_shader final
{
public:

  explicit          Material_shader();
  explicit          Material_shader(const char *filepath);
  explicit          Material_shader(const char *vs, const char *gs, const char *ps);
  
                    ~Material_shader();
  
  bool              is_valid() const;

  bool              has_map_01() const;
  bool              has_map_02() const;
  bool              has_map_03() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};


} // ns


#endif // inc guard