// capnproto::read(std::istream&, const Schema<T>&) — infinite recursion
// File: include/rfl/capnproto/read.hpp:79-82
//
// The overload calls itself recursively. GCC detects this as
// "use of auto before deduction of auto". This file attempts to
// instantiate the buggy overload. It should FAIL to compile while
// the bug exists, and succeed after the fix.

#include <sstream>
#include <string>

#include <rfl.hpp>
#include <rfl/capnproto.hpp>

struct Simple {
  std::string field1;
  int field2;
};

int main() {
  const auto schema = rfl::capnproto::to_schema<Simple>();
  std::istringstream stream("dummy");
  auto res = rfl::capnproto::read<Simple>(stream, schema);
  return 0;
}
