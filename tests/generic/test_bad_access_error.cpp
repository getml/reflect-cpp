#ifdef REFLECTCPP_USE_STD_EXPECTED
#include <expected>
#endif

#include <rfl.hpp>
#include <gtest/gtest.h>

namespace test_exceptions {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Flanders";
};

TEST(generic, test_exceptions) {
  const std::string error_message = "unified bad access message!";
  #ifdef REFLECTCPP_USE_STD_EXPECTED
  rfl::Result<Person> person = std::unexpected(rfl::Error(error_message));
#else
  rfl::Result<Person> person(rfl::error(error_message));
#endif

  try {
    person.value();
    FAIL() << "Expected an exception!";
  }
#ifdef REFLECTCPP_USE_STD_EXPECTED
  catch (std::bad_expected_access<rfl::Error> const& err) {
    EXPECT_EQ(err.what(), error_message);
  }
#else
  catch (std::runtime_error const& err) {
    EXPECT_EQ(err.what(), error_message);
  }
#endif
  catch (...) {
    FAIL() << "Invalid exception type!";
  }
}
}  // namespace test_exceptions
