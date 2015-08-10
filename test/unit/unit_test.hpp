#ifndef UNIT_TEST_INCLUDED_AA1384FD_8BEA_4624_857F_00D0B38A3059
#define UNIT_TEST_INCLUDED_AA1384FD_8BEA_4624_857F_00D0B38A3059


#include <list>
#include <string>
#include <math.h>
#include <iostream>
#include <stdint.h>


namespace test {


/*
 *  Interface class for the tests.
 */
struct Test_interface
{
  virtual std::string get_name() const = 0;
  virtual void run(std::ostream & stream) const = 0;
}; // end of struct




/*
 *  Unit tests, this is a singleton class, access through instance();
 */
class Unit_test
{
private:

  uint32_t                   m_passed = 0;
  uint32_t                   m_failed = 0;
  std::string                m_test_errors = "";
  std::list<Test_interface*> m_tests;     

  // Singleton so hiding access
  Unit_test(const Unit_test &);
  Unit_test& operator=(const Unit_test&);

  // Private ctor.
  explicit Unit_test()
    : m_passed(0)
    , m_failed(0)
    , m_tests()
  {}

public:

  // Singlton access.
  static Unit_test& instance()
  {
    static Unit_test test;
    return test;
  }


  // This adds a test to the list.
  void add_test(Test_interface *new_test)
  {
    m_tests.push_back(new_test);  
  }


  // Runs all the tests.
  void run_tests(std::ostream & stream)
  {
    // Title screen.
    stream << "\nRunning Unit Tests\n";
    stream << "==================\n";

    // Run the individual tests.
    for(auto &test : m_tests)
    {
      uint32_t fails(m_failed);

      stream << "\n" << test->get_name() << "\n";
      test->run(stream);

      if(fails == m_failed)
      {
        stream << "\nAll test(s) passed!";
      }
      else
      {
        stream << "\n" << (m_failed - fails) << " Test(s) failed!";
        stream << m_test_errors;
      }
      m_test_errors = "";
      stream << "\n--";

    } // for

    // End screen.
    stream << "\n\nResults\n-------\n";
    stream << "Tests Passed: " << m_passed << "\n";
    stream << "Tests Failed: " << m_failed << "\n";

  } // void runTests(...)


  // Increase the passed tests.
  void passed_current_test() { m_passed++; }
  // Increase the failed tests.
  void failed_current_test() { m_failed++; }
  // Add error message.
  void add_error_message(const std::string &err)
  {
  	m_test_errors += "\n" + err;
  }

}; // end of class


// Run the tests
void run_tests(std::ostream & stream = std::cout)
{
  test::Unit_test::instance().run_tests(stream);
}

} // namespace


// *** TEST METHOD *** //

// Creates a test case.
#define TEST(ClassName)                                         \
struct Test##ClassName : public test::Test_interface            \
{                                                               \
  std::string name;                                             \
  Test##ClassName()                                             \
  : name(#ClassName)                                            \
  {                                                             \
    test::Unit_test::instance().add_test(this);                 \
  }                                                             \
                                                                \
  std::string get_name() const { return name; }                 \
                                                                \
  void run(std::ostream & test_stream) const;                   \
} instanceTest##ClassName;                                      \
                                                                \
void Test##ClassName::run(std::ostream & test_stream) const     \


#define CHECK(test_pass)                                                             \
{                                                                                    \
  if (test_pass)                                                                     \
  {                                                                                  \
    test::Unit_test::instance().passed_current_test();                               \
    test_stream << ".";                                                              \
  }                                                                                  \
  else                                                                               \
  {                                                                                  \
    test::Unit_test::instance().failed_current_test();                               \
    test_stream << "x";                                                              \
  }                                                                                  \
}                                                                                    \


// *** TEST Methods *** //

namespace test {


template<typename T>
bool is_equal(const T &a, const T &b)
{
  return a == b;
}


template<typename T>
bool is_not_equal(const T &a, const T &b)
{
  return (a != b);
}


template<typename T>
bool is_near(const T &a, const T &b, const float near)
{
  return ((fabs(a - b)) <= near);
}


template<typename T>
bool is_not_near(const T &a, const T &b, const float near)
{
  return ((fabs(a - b)) >= near);
}


template<typename T>
bool is_true(const T &a)
{
  return(a);
}


template<typename T>
bool is_false(const T &a)
{
  return(!a);
}


template<typename T>
bool is_greater(const T &a, const T &b)
{
  return a > b;
}


template<typename T>
bool is_less(const T &a, const T &b)
{
  return a < b;
}


} // namespace


#endif // include guard