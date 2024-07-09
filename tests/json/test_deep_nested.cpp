#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_deep_nested {

class LowHeightClassSub {
 public:
  double dummy1 = 0;
  double dummy2 = 0;
  double dummy3 = 0;
  double dummy4 = 0;
  double dummy5 = 0;
  double dummy6 = 0;
  double dummy7 = 0;
  double dummy8 = 0;
  double dummy9 = 0;
  double dummy10 = 0;
  double dummy11 = 0;
  double dummy12 = 0;
  double dummy13 = 0;
  double dummy14 = 0;
  double dummy15 = 0;
  double dummy16 = 0;
  double dummy17 = 0;
  double dummy18 = 0;
  double dummy19 = 0;
  double dummy20 = 0;
  double dummy21 = 0;
};

class LowHeightClass {
 public:
  double dummy1 = 0;
  double dummy2 = 0;
  double dummy3 = 0;

  double dummy4 = 0;
  double dummy5 = 0;
  double dummy6 = 0;

  double dummy7 = 0;
  double dummy8 = 0;

  double dummy9 = 0;
  double dummy10 = 0;

  double dummy11 = 0;
  double dummy12 = 0;
  double dummy13 = 0;
  double dummy14 = 0;

  LowHeightClassSub dummy15{};
  LowHeightClassSub dummy16{};
  LowHeightClassSub dummy17{};
  LowHeightClassSub dummy18{};
};

class DeepClass {
 public:
  LowHeightClass LHC;

  int dummy1;
  int dummy2;
  double dummy3;
};

TEST(json, test_deep_nested) {
  DeepClass dc{};

  write_and_read(
      dc,
      R"({"LHC":{"dummy1":0.0,"dummy2":0.0,"dummy3":0.0,"dummy4":0.0,"dummy5":0.0,"dummy6":0.0,"dummy7":0.0,"dummy8":0.0,"dummy9":0.0,"dummy10":0.0,"dummy11":0.0,"dummy12":0.0,"dummy13":0.0,"dummy14":0.0,"dummy15":{"dummy1":0.0,"dummy2":0.0,"dummy3":0.0,"dummy4":0.0,"dummy5":0.0,"dummy6":0.0,"dummy7":0.0,"dummy8":0.0,"dummy9":0.0,"dummy10":0.0,"dummy11":0.0,"dummy12":0.0,"dummy13":0.0,"dummy14":0.0,"dummy15":0.0,"dummy16":0.0,"dummy17":0.0,"dummy18":0.0,"dummy19":0.0,"dummy20":0.0,"dummy21":0.0},"dummy16":{"dummy1":0.0,"dummy2":0.0,"dummy3":0.0,"dummy4":0.0,"dummy5":0.0,"dummy6":0.0,"dummy7":0.0,"dummy8":0.0,"dummy9":0.0,"dummy10":0.0,"dummy11":0.0,"dummy12":0.0,"dummy13":0.0,"dummy14":0.0,"dummy15":0.0,"dummy16":0.0,"dummy17":0.0,"dummy18":0.0,"dummy19":0.0,"dummy20":0.0,"dummy21":0.0},"dummy17":{"dummy1":0.0,"dummy2":0.0,"dummy3":0.0,"dummy4":0.0,"dummy5":0.0,"dummy6":0.0,"dummy7":0.0,"dummy8":0.0,"dummy9":0.0,"dummy10":0.0,"dummy11":0.0,"dummy12":0.0,"dummy13":0.0,"dummy14":0.0,"dummy15":0.0,"dummy16":0.0,"dummy17":0.0,"dummy18":0.0,"dummy19":0.0,"dummy20":0.0,"dummy21":0.0},"dummy18":{"dummy1":0.0,"dummy2":0.0,"dummy3":0.0,"dummy4":0.0,"dummy5":0.0,"dummy6":0.0,"dummy7":0.0,"dummy8":0.0,"dummy9":0.0,"dummy10":0.0,"dummy11":0.0,"dummy12":0.0,"dummy13":0.0,"dummy14":0.0,"dummy15":0.0,"dummy16":0.0,"dummy17":0.0,"dummy18":0.0,"dummy19":0.0,"dummy20":0.0,"dummy21":0.0}},"dummy1":0,"dummy2":0,"dummy3":0.0})");
}

}  // namespace test_deep_nested
