#include "write_and_read.hpp"
#include <rfl.hpp>
void test_constr() {

  std::cout << "test_constr" << std::endl;

  // Custom validation for string
  // Masked credit cards
  using MaskedCreditCardType = rfl::ConStr<R"(XXXX-XXXX-XXXX-\d{4})">;
  // Nick name between 5 and 10 characters long without regex use
  using NickNameType = rfl::ConStr<"", 5, 10>;

  struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name = "Simpson";
    rfl::Field<"id", rfl::UUIDv4> id;
    rfl::Field<"payload", rfl::Base64Encoded> payload;
    rfl::Field<"maskedCreditCard", MaskedCreditCardType> masked_credit_card;
    rfl::Field<"nickName", NickNameType> nick_name;
  };

  const auto bart = Person{
      .first_name = "Bart",
      .id = "de76b905-5b32-4127-96d1-48e5321e11a6",
      .payload = "aGVsbG93b3JsZAo=",
      .masked_credit_card = "XXXX-XXXX-XXXX-4440",
      .nick_name = "Cosmo",

  };

  write_and_read(bart, "{\"firstName\":\"Bart\",\"lastName\":\"Simpson\","
                       "\"id\":\"de76b905-5b32-4127-96d1-48e5321e11a6\","
                       "\"payload\":\"aGVsbG93b3JsZAo=\","
                       "\"maskedCreditCard\":\"XXXX-XXXX-XXXX-4440\","
                       "\"nickName\":\"Cosmo\"}");
}
