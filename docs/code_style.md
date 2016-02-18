# Code Style

General coding style **guidlines**

### Naming Conventions

```cpp
namespace Foo_bar {

int
bar_foo();

class Bar_foo
{
public:

  void        boo();
  bool        baz();

};


Bar_foo boo_hoo;

} // ns
```


### Curly Braces

- Place namespace curlies inline.
- No restriction on new line or line for methods and functions.



### References / Pointers

Prefer pointers over refs.



### Naked Allocations

- Prefer smart_pointers over raw allocs.
- Prefer `unique_ptr` over `shared_ptr`.