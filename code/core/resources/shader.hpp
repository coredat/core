#ifndef SHADER_INCLUDED_987AF2DE_4AAB_47E3_9742_51C7B5433AB0
#define SHADER_INCLUDED_987AF2DE_4AAB_47E3_9742_51C7B5433AB0


#include <memory>


namespace Core {


class Shader
{
public:

  explicit                  Shader(const char *filename);
  
                            ~Shader();
  
  bool                      is_valid() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl>     m_impl;
  
};


} // ns


#endif // inc guard