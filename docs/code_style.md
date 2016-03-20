# Code Style

General coding style *guidlines*

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

- No restriction on new line or inline for methods and functions.
- Namespace must be inline.


### References / Pointers

Prefer pointers over refs.



### Naked Allocations

- Prefer smart_pointers over raw allocs.
- Prefer `unique_ptr` over `shared_ptr`.
