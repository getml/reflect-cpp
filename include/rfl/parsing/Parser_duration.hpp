#ifndef RFL_PARSING_PARSER_DURATION_HPP_
#define RFL_PARSING_PARSER_DURATION_HPP_

#include <chrono>
#include <map>
#include <optional>
#include <type_traits>

#include "../Literal.hpp"
#include "../Variant.hpp"
#include "../always_false.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl ::parsing {

template <class R, class W, class Rep, class Period, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::chrono<Rep, Period>>
struct Parser<R, W, std::chrono<Rep, Period>, ProcessorsType> {
 public:
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  using ChronoType = std::chrono<Rep, Period>;

  using Unit = Literal<"nanoseconds", "microseconds", "milliseconds", "seconds",
                       "minutes", "hours", "days", "weeks", "months", "years">;

  using SupportedTypes =
      Variant<std::chrono::nanoseconds, std::chrono::microseconds,
              std::chrono::milliseconds, std::chrono::seconds,
              std::chrono::minutes, std::chrono::hours, std::chrono::days,
              std::chrono::weeks, std::chrono::months, std::chrono::years>;

  struct ReflectionType {
    int64_t count;
    Unit unit;
  };

  static Result<std::chrono<Rep, Period>> read(
      const R& _r, const InputVarType& _var) noexcept {
    return Parser<R, W, ReflectionType, ProcessorsType>::read(_r, _var)
        .transform(to_chrono)
        .transform([](auto&& _chrono) {
          return _chrono.visit(
              [](auto&& _c) { return ChronoType(std::move(_c)); });
        });
  }

  template <class P>
  static void write(const W& _w, const std::chrono<Rep, Period>& _c,
                    const P& _parent) noexcept {
    const auto r = ReflectionType{.count = static_cast<int64_t>(_c.count()),
                                  .unit = make_unit()};
    return Parser<R, W, ReflectionType, ProcessorsType>::write(_w, r, _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, ReflectionType, ProcessorsType>::to_schema(
        _definitions);
  }

 private:
  static SupportedTypes to_chrono(const ReflectionType& _r) {
    switch (_r.unit.value()) {
      case Unit::value_of<"nanoseconds">:
        return std::chrono::nanoseconds(_r.count);

      case Unit::value_of<"microseconds">:
        return std::chrono::microseconds(_r.count);

      case Unit::value_of<"milliseconds">:
        return std::chrono::milliseconds(_r.count);

      case Unit::value_of<"seconds">:
        return std::chrono::seconds(_r.count);

      case Unit::value_of<"minutes">:
        return std::chrono::minutes(_r.count);

      case Unit::value_of<"days">:
        return std::chrono::days(_r.count);

      case Unit::value_of<"weeks">:
        return std::chrono::weeks(_r.count);

      case Unit::value_of<"months">:
        return std::chrono::months(_r.count);

      case Unit::value_of<"years">:
        return std::chrono::years(_r.count);

      default:
        return std::chrono::seconds(_r.count);
    }
  }

  static auto make_unit() noexcept {
    if constexpr (std::is_same_v<ChronoType, std::chrono::nanoseconds>) {
      return Unit::make<"nanoseconds">();

    } else if constexpr (std::is_same_v<ChronoType,
                                        std::chrono::microseconds>) {
      return Unit::make<"microseconds">();

    } else if constexpr (std::is_same_v<ChronoType,
                                        std::chrono::milliseconds>) {
      return Unit::make<"milliseconds">();

    } else if constexpr (std::is_same_v<ChronoType, std::chrono::seconds>) {
      return Unit::make<"seconds">();

    } else if constexpr (std::is_same_v<ChronoType, std::chrono::minutes>) {
      return Unit::make<"minutes">();

    } else if constexpr (std::is_same_v<ChronoType, std::chrono::hours>) {
      return Unit::make<"hours">();

    } else if constexpr (std::is_same_v<ChronoType, std::chrono::days>) {
      return Unit::make<"days">();

    } else if constexpr (std::is_same_v<ChronoType, std::chrono::weeks>) {
      return Unit::make<"weeks">();

    } else if constexpr (std::is_same_v<ChronoType, std::chrono::months>) {
      return Unit::make<"months">();

    } else if constexpr (std::is_same_v<ChronoType, std::chrono::years>) {
      return Unit::make<"years">();

    } else {
      static_assert(always_false_v<ChronoType>, "Unsupported type.");
    }
  };
};

}  // namespace rfl::parsing

#endif
