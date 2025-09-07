#include <algorithm>
#include <array>
#include <rfl/ubjson.hpp>

#include <gtest/gtest.h>

// Make sure things still compile when
// rfl.hpp is included after rfl/ubjson.hpp.
#include <rfl.hpp>

namespace test_read_byte_containers
{

struct TestBall
{
    float radius;
    float mass;
};

TEST(ubjson, test_read_from_byte_view)
{
    TestBall b = {
        .radius = 1.5f,
        .mass = 2.5f,
    };

    // TODO: Write directly into desired container, once rfl::ubjson::write supports it.
    std::vector<char> rfl_buffer = rfl::ubjson::write(b);

    std::array<std::byte, 64> my_buffer;
    std::transform(rfl_buffer.begin(), rfl_buffer.end(), my_buffer.begin(),
        [](char c) { return static_cast<std::byte>(c); });

    std::basic_string_view<std::byte> byte_view(my_buffer.data(), rfl_buffer.size());

    auto result = rfl::ubjson::read<TestBall>(byte_view);
    EXPECT_TRUE(result);
    EXPECT_EQ(result->radius, 1.5f);
    EXPECT_EQ(result->mass, 2.5f);
}

TEST(ubjson, test_read_from_uint8_array)
{
    TestBall b = {
        .radius = 4.5f,
        .mass = 5.5f,
    };

    // TODO: Write directly into desired container, once rfl::ubjson::write supports it.
    std::vector<char> rfl_buffer = rfl::ubjson::write(b);
    
    std::array<std::uint8_t, 64> my_buffer;
    std::transform(rfl_buffer.begin(), rfl_buffer.end(), my_buffer.begin(),
        [](char c) { return static_cast<std::uint8_t>(c); });

    auto result = rfl::ubjson::read<TestBall>(my_buffer);
    EXPECT_TRUE(result);
    EXPECT_EQ(result->radius, 4.5f);
    EXPECT_EQ(result->mass, 5.5f);
}

} // namespace test_read_byte_containers
