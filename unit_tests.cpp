#define BOOST_TEST_MODULE bulk_test_module

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <iostream>

#include "bulk.h"

BOOST_AUTO_TEST_SUITE(bulk_test_suite)

/**
 * @brief Helper redirection fot tests
 */
struct cout_redirect
{
  cout_redirect(std::streambuf *new_buffer)
      : old(std::cout.rdbuf(new_buffer))
  {
  }

  ~cout_redirect()
  {
    std::cout.rdbuf(old);
  }

private:
  std::streambuf *old;
};

BOOST_AUTO_TEST_CASE(bulk_input_test_1)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());

    Notifier notifier(2);

    auto display_output = std::make_unique<DisplayOutput>(); 

    notifier.subscribe(display_output.get());

    std::stringstream ss;
    ss << std::string("c1") << std::endl
       << std::string("c2") << std::endl
       << std::string("c3") << std::endl
       << std::string("c4") << std::endl;

    notifier.get_stream_input(ss);
  }
  BOOST_CHECK(output.is_equal("bulk: c1 c2\nbulk: c3 c4\n"));
}

BOOST_AUTO_TEST_CASE(bulk_input_test_2)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());

    Notifier notifier(1);

    auto display_output = std::make_unique<DisplayOutput>(); 

    notifier.subscribe(display_output.get());

    std::stringstream ss;
    ss << std::string("c1") << std::endl
       << std::string("c2") << std::endl
       << std::string("c3") << std::endl
       << std::string("c4") << std::endl;

    notifier.get_stream_input(ss);
  }
  BOOST_CHECK(output.is_equal("bulk: c1\nbulk: c2\nbulk: c3\nbulk: c4\n"));
}

BOOST_AUTO_TEST_CASE(bulk_input_test_3)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());

    Notifier notifier(3);

    auto display_output = std::make_unique<DisplayOutput>(); 

    notifier.subscribe(display_output.get());

    std::stringstream ss;
    ss << std::string("c1") << std::endl
       << std::string("c2") << std::endl
       << std::string("c3") << std::endl
       << std::string("{") << std::endl
       << std::string("c4") << std::endl
       << std::string("c5") << std::endl
       << std::string("c6") << std::endl
       << std::string("c7") << std::endl
       << std::string("}") << std::endl;

    notifier.get_stream_input(ss);
  }
  BOOST_CHECK(output.is_equal("bulk: c1 c2 c3\nbulk: c4 c5 c6 c7\n"));
}

BOOST_AUTO_TEST_CASE(bulk_input_test_4)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());

    Notifier notifier(3);

    auto display_output = std::make_unique<DisplayOutput>(); 

    notifier.subscribe(display_output.get());

    std::stringstream ss;
    ss << std::string("{") << std::endl
       << std::string("c1") << std::endl
       << std::string("c2") << std::endl
       << std::string("{") << std::endl
       << std::string("c3") << std::endl
       << std::string("c4") << std::endl
       << std::string("}") << std::endl
       << std::string("c5") << std::endl
       << std::string("c6") << std::endl
       << std::string("}") << std::endl;

    notifier.get_stream_input(ss);
  }
  BOOST_CHECK(output.is_equal("bulk: c1 c2 c3 c4 c5 c6\n"));
}

BOOST_AUTO_TEST_CASE(bulk_input_test_5)
{
  boost::test_tools::output_test_stream output;
  {
    cout_redirect redirect(output.rdbuf());

    Notifier notifier(3);

    auto display_output = std::make_unique<DisplayOutput>(); 

    notifier.subscribe(display_output.get());

    std::stringstream ss;
    ss << std::string("c1") << std::endl
       << std::string("c2") << std::endl
       << std::string("c3") << std::endl
       << std::string("{")  << std::endl
       << std::string("c4") << std::endl
       << std::string("c5") << std::endl
       << std::string("c6") << std::endl
       << std::string("c7") << std::endl;

    notifier.get_stream_input(ss);
  }
  BOOST_CHECK(output.is_equal("bulk: c1 c2 c3\n"));
}

BOOST_AUTO_TEST_SUITE_END()
