#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <rfl.hpp>
#include <rfl/boost_serialization.hpp>
#include <sstream>
#include <string>

#include "write_and_read.hpp"

namespace test_archive_interop {

struct Person {
  std::string first_name;
  std::string last_name;
  int age;
};

TEST(boost_serialization, test_binary_archive_interop) {
  const auto homer =
      Person{.first_name = "Homer", .last_name = "Simpson", .age = 45};

  std::ostringstream os;
  {
    boost::archive::binary_oarchive oa(os);
    rfl::boost_serialization::write(oa, homer);
  }

  const auto str = os.str();
  std::istringstream is(str);
  boost::archive::binary_iarchive ia(is);
  const auto res = rfl::boost_serialization::read_from_archive<
      Person, boost::archive::binary_iarchive, boost::archive::binary_oarchive>(
      ia);
  EXPECT_TRUE(res && true) << "Error: " << res.error().what();
  EXPECT_EQ(res.value().first_name, "Homer");
  EXPECT_EQ(res.value().last_name, "Simpson");
  EXPECT_EQ(res.value().age, 45);
}

TEST(boost_serialization, test_text_archive_round_trip) {
  const auto homer =
      Person{.first_name = "Homer", .last_name = "Simpson", .age = 45};

  std::ostringstream os;
  {
    boost::archive::text_oarchive oa(os);
    rfl::boost_serialization::write(oa, homer);
  }

  const auto str = os.str();
  std::istringstream is(str);
  boost::archive::text_iarchive ia(is);
  const auto res = rfl::boost_serialization::read_from_archive<
      Person, boost::archive::text_iarchive, boost::archive::text_oarchive>(ia);
  EXPECT_TRUE(res && true) << "Error: " << res.error().what();
  EXPECT_EQ(res.value().first_name, "Homer");
  EXPECT_EQ(res.value().last_name, "Simpson");
  EXPECT_EQ(res.value().age, 45);
}

}  // namespace test_archive_interop
