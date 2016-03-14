#define CATCH_CONFIG_MAIN
#include "test_framework/catch.hpp"

#include <iostream>


#ifndef CATCH_CONFIG_MAIN
int
main()
{
  std::cout << "Test" << std::endl;
  return 0;
}
#endif // inc guard