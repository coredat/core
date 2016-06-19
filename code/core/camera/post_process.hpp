#ifndef POST_PROCESS_INCLUDED_D44ABCD3_122A_49D4_B40C_801091A802D1
#define POST_PROCESS_INCLUDED_D44ABCD3_122A_49D4_B40C_801091A802D1


#include <core/common/core_fwd.hpp>
#include <core/common/core_types.hpp>


namespace Core {


class Post_process
{
public:

  explicit      Post_process(const char *name);
                ~Post_process();
  
  void          set_shader(const Shader &shader);
  Shader        get_shader() const;
  
  void          set_input_01(const Texture &texture);
  Texture       get_input_01() const;

  void          set_input_02(const Texture &texture);
  Texture       get_input_02() const;

  void          set_input_03(const Texture &texture);
  Texture       get_input_03() const;
  
  uint32_t      get_id() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

};


} // ns


#endif // inc guard