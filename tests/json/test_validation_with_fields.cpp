#include <cassert>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_validation_with_fields {

using SimpleNameValidator = rfl::Validator<
    std::string,
    rfl::AllOf<rfl::PatternValidator<R"(^[A-Za-z1-9_]+$)", "TagName">>,
    rfl::Size<rfl::Minimum<3>>, rfl::Size<rfl::Maximum<50>>>;

struct TagConfig {
  SimpleNameValidator Name;
  int32_t Length;
  bool WriteEnabled;
};

struct ThreadConfig {
  rfl::Field<"type", SimpleNameValidator> Type;
  rfl::Field<"Name", SimpleNameValidator> Name;
  rfl::Field<"ReadTags", std::vector<TagConfig>> ReadTags;
  rfl::Field<"WriteTags", std::vector<TagConfig>> WriteTags;
  rfl::Field<"ThreadPriority", int32_t> ThreadPriority;
  rfl::Field<"DecimationFactor", int32_t> DecimationFactor;
  rfl::Field<"TimingOffset", int32_t> TimingOffset;
};

TEST(json, test_validation_with_fields) {
  const auto read_tag1 =
      TagConfig{.Name = "READ_TAG_1", .Length = 5, .WriteEnabled = false};

  const auto write_tag1 =
      TagConfig{.Name = "WRITE_TAG_1", .Length = 10, .WriteEnabled = true};

  const auto test_thread_config =
      ThreadConfig{.Type = "BasicThread",
                   .Name = "TEST_1",
                   .ReadTags = std::vector<TagConfig>({read_tag1}),
                   .WriteTags = std::vector<TagConfig>({write_tag1}),
                   .ThreadPriority = -1,
                   .DecimationFactor = 10,
                   .TimingOffset = 0};

  write_and_read(
      test_thread_config,
      R"({"type":"BasicThread","Name":"TEST_1","ReadTags":[{"Name":"READ_TAG_1","Length":5,"WriteEnabled":false}],"WriteTags":[{"Name":"WRITE_TAG_1","Length":10,"WriteEnabled":true}],"ThreadPriority":-1,"DecimationFactor":10,"TimingOffset":0})");
}

}  // namespace test_validation_with_fields
