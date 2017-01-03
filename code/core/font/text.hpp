#ifndef TEXT_INCLUDED_D6CB6430_256E_4984_8897_6E89E4BF3F03
#define TEXT_INCLUDED_D6CB6430_256E_4984_8897_6E89E4BF3F03


namespace Core {


class Text final
{
public:

  explicit        Text();
  explicit        Text(const char *text);
  
  const char *    get_stripped_text() const;
  const char *    get_text() const;
  
  // Alias's
  
  const char *    c_str_strip() const;
  const char *    c_str() const;
  
private:

  

};


} // ns


#endif // inc guard