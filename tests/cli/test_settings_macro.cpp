#include <gtest/gtest.h>
#include <rfl/Settings.hpp>
#include <string>
#include <utility>

import rfl;


namespace test_settings_macro {

struct DemoSettings {
  const int batch_size = 1024;
  const char delimiter = ',';
  const std::string null_string = "n/a";
  const bool quoting = true;

  RFL_SETTINGS_OPS(DemoSettings)
};

TEST(settings_macro, single_field_replace_returns_new_copy) {
  const DemoSettings original{};
  const auto modified =
      original.with<&DemoSettings::delimiter>(';');

  EXPECT_EQ(original.delimiter, ',');
  EXPECT_EQ(modified.delimiter, ';');
  EXPECT_EQ(modified.batch_size, original.batch_size);
  EXPECT_EQ(modified.null_string, original.null_string);
  EXPECT_EQ(modified.quoting, original.quoting);
}

TEST(settings_macro, chained_with_calls_apply_all_replacements) {
  const auto modified = DemoSettings{}
                            .with<&DemoSettings::batch_size>(2048)
                            .with<&DemoSettings::delimiter>('|')
                            .with<&DemoSettings::null_string>(std::string{"NA"})
                            .with<&DemoSettings::quoting>(false);

  EXPECT_EQ(modified.batch_size, 2048);
  EXPECT_EQ(modified.delimiter, '|');
  EXPECT_EQ(modified.null_string, "NA");
  EXPECT_FALSE(modified.quoting);
}

TEST(settings_macro, replace_string_field_moves_value) {
  // The with<> parameter is passed by value, so passing an rvalue lets the
  // implementation move into make_field. Result must equal the source.
  std::string moved_in = "moved-in-value";
  const auto modified =
      DemoSettings{}.with<&DemoSettings::null_string>(std::move(moved_in));
  EXPECT_EQ(modified.null_string, "moved-in-value");
}

TEST(settings_macro, by_name_replace_works_for_each_field) {
  const auto modified = DemoSettings{}.with<"delimiter">(';');
  EXPECT_EQ(modified.delimiter, ';');
  EXPECT_EQ(modified.batch_size, 1024);
}

TEST(settings_macro, by_name_chained_calls_apply_all_replacements) {
  const auto modified = DemoSettings{}
                            .with<"batch_size">(4096)
                            .with<"delimiter">('\t')
                            .with<"null_string">(std::string{"NULL"})
                            .with<"quoting">(false);
  EXPECT_EQ(modified.batch_size, 4096);
  EXPECT_EQ(modified.delimiter, '\t');
  EXPECT_EQ(modified.null_string, "NULL");
  EXPECT_FALSE(modified.quoting);
}

TEST(settings_macro, by_name_and_by_ptm_are_interchangeable) {
  const auto by_ptm = DemoSettings{}.with<&DemoSettings::delimiter>(';');
  const auto by_name = DemoSettings{}.with<"delimiter">(';');
  EXPECT_EQ(by_ptm.delimiter, by_name.delimiter);
  EXPECT_EQ(by_ptm.batch_size, by_name.batch_size);
}

}  // namespace test_settings_macro
