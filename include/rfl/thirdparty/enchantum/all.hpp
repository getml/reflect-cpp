#pragma once
#include "details/optional.hpp"
#include "details/string_view.hpp"
#include "details/generate_arrays.hpp"

#include "algorithms.hpp"
#include "array.hpp"
#include "bitflags.hpp"
#include "bitwise_operators.hpp"
#include "enchantum.hpp"
#include "entries.hpp"
#include "iostream.hpp"
#include "next_value.hpp"
#include "bitset.hpp"

#if __has_include(<fmt/format.h>)
  #include "fmt_format.hpp"
#elif __has_include(<format>)
  #include "std_format.hpp"
#endif