#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_many_fields {

class ManyFields {
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
  double dummy22 = 0;
  double dummy23 = 0;
  double dummy24 = 0;
  double dummy25 = 0;
  double dummy26 = 0;
  double dummy27 = 0;
  double dummy28 = 0;
  double dummy29 = 0;
  double dummy30 = 0;
  double dummy31 = 0;
  double dummy32 = 0;
  double dummy33 = 0;
  double dummy34 = 0;
  double dummy35 = 0;
  double dummy36 = 0;
  double dummy37 = 0;
  double dummy38 = 0;
  double dummy39 = 0;
  double dummy40 = 0;
  double dummy41 = 0;
  double dummy42 = 0;
  double dummy43 = 0;
  double dummy44 = 0;
  double dummy45 = 0;
  double dummy46 = 0;
  double dummy47 = 0;
  double dummy48 = 0;
  double dummy49 = 0;
  double dummy50 = 0;
  double dummy51 = 0;
  double dummy52 = 0;
  double dummy53 = 0;
  double dummy54 = 0;
  double dummy55 = 0;
  double dummy56 = 0;
  double dummy57 = 0;
  double dummy58 = 0;
  double dummy59 = 0;
  double dummy60 = 0;
  double dummy61 = 0;
  double dummy62 = 0;
  double dummy63 = 0;
  double dummy64 = 0;
  double dummy65 = 0;
  double dummy66 = 0;
  double dummy67 = 0;
  double dummy68 = 0;
  double dummy69 = 0;
  double dummy70 = 0;
};

TEST(json, test_many_fields) {
  ManyFields mf{};

  write_and_read(
      mf,
      R"({"dummy1":0.0,"dummy2":0.0,"dummy3":0.0,"dummy4":0.0,"dummy5":0.0,"dummy6":0.0,"dummy7":0.0,"dummy8":0.0,"dummy9":0.0,"dummy10":0.0,"dummy11":0.0,"dummy12":0.0,"dummy13":0.0,"dummy14":0.0,"dummy15":0.0,"dummy16":0.0,"dummy17":0.0,"dummy18":0.0,"dummy19":0.0,"dummy20":0.0,"dummy21":0.0,"dummy22":0.0,"dummy23":0.0,"dummy24":0.0,"dummy25":0.0,"dummy26":0.0,"dummy27":0.0,"dummy28":0.0,"dummy29":0.0,"dummy30":0.0,"dummy31":0.0,"dummy32":0.0,"dummy33":0.0,"dummy34":0.0,"dummy35":0.0,"dummy36":0.0,"dummy37":0.0,"dummy38":0.0,"dummy39":0.0,"dummy40":0.0,"dummy41":0.0,"dummy42":0.0,"dummy43":0.0,"dummy44":0.0,"dummy45":0.0,"dummy46":0.0,"dummy47":0.0,"dummy48":0.0,"dummy49":0.0,"dummy50":0.0,"dummy51":0.0,"dummy52":0.0,"dummy53":0.0,"dummy54":0.0,"dummy55":0.0,"dummy56":0.0,"dummy57":0.0,"dummy58":0.0,"dummy59":0.0,"dummy60":0.0,"dummy61":0.0,"dummy62":0.0,"dummy63":0.0,"dummy64":0.0,"dummy65":0.0,"dummy66":0.0,"dummy67":0.0,"dummy68":0.0,"dummy69":0.0,"dummy70":0.0})");
}

}  // namespace test_many_fields
