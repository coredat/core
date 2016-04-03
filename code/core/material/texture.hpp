#ifndef TEXTURE_INCLUDED_4BAC239C_846B_4C57_9B2D_F49FB4E96062
#define TEXTURE_INCLUDED_4BAC239C_846B_4C57_9B2D_F49FB4E96062


#include <stdint.h>
#include <memory>


namespace Core {


class Texture
{
public:

  explicit            Texture();
  explicit            Texture(const uint32_t id);
  explicit            Texture(const char * filepath);
  
                      ~Texture();
  
                      Texture(const Texture&);
                      Texture(Texture&&);
  
  bool                exists() const;
                      operator bool() const; // Alias for exists().
  
  uint32_t            get_id() const;
  
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;

}; // class


} // ns


#endif // inc guard