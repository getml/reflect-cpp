#include <map>
#include <string>

#include "write_and_read.hpp"
import rfl;
import rfl.json;


namespace test_wstring {
TEST(json, test_wstring) {
  std::map<std::string, std::wstring> homer;
  homer.insert(std::make_pair("firstName", L"Homer"));

  write_and_read(homer, R"({"firstName":"Homer"})");
}

struct WithWstrings {
  std::wstring name;
  std::wstring note;
};

// Regression test for issues #421 and #422: non-ASCII content previously made
// std::mbsrtowcs / std::wcsrtombs return (size_t)-1, which was passed unchecked
// to resize() and terminated the process. (This file is UTF-8 encoded;
// astral-plane characters use \U escapes.)
TEST(json, test_wstring_non_ascii) {
  // Latin-1 supplement (é), plus an empty wstring.
  write_and_read(WithWstrings{.name = L"René", .note = L""},
                 R"({"name":"René","note":""})");

  // Cyrillic ("Привіт"), CJK and Hiragana ("中文 ひらがな").
  write_and_read(
      WithWstrings{.name = L"Привіт",
                   .note = L"中文 ひらがな"},
      R"({"name":"Привіт","note":"中文 ひらがな"})");

  // Code points beyond the BMP (surrogate pairs where wchar_t is 16 bits wide):
  // U+1F4A9 and U+1F600.
  write_and_read(WithWstrings{.name = L"\U0001f4a9", .note = L"a\U0001f600b"},
                 R"({"name":"💩","note":"a😀b"})");
}
}  // namespace test_wstring
