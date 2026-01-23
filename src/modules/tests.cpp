import rfl;

#include <string>
#include <type_traits>
#include <vector>

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::string town = "Springfield";
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  unsigned int age;
  rfl::Email email;
  std::vector<Person> child;
};

void test_json() {
  const auto bart = Person{.first_name = "Bart",
                           .birthday = "1987-04-19",
                           .age = 10,
                           .email = "bart@simpson.com"};

  const auto lisa = Person{.first_name = "Lisa",
                           .birthday = "1987-04-19",
                           .age = 8,
                           .email = "lisa@simpson.com"};

  const auto maggie = Person{.first_name = "Maggie",
                             .birthday = "1987-04-19",
                             .age = 0,
                             .email = "maggie@simpson.com"};

  const auto homer = Person{.first_name = "Homer",
                            .birthday = "1987-04-19",
                            .age = 45,
                            .email = "homer@simpson.com",
                            .child = std::vector<Person>({bart, lisa, maggie})};
}

int main() {
  test_json();
  return 0;
}
