//#define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_MAIN
#include "test_framework/catch.hpp"

#include <iostream>


#ifndef CATCH_CONFIG_MAIN

int
main(int argc, const char** argv)
{
  Catch::Session session;

  int returnCode = session.run(argc, argv);

  return returnCode;
}

#endif