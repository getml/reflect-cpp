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
  requires AreReaderAndWriter<R, W, std::chrono::duration<Rep, Period>>
struct Parser<R, W, std::chrono::duration<Rep, Period>, ProcessorsType> {
 public:
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  using DurationType = std::chrono::duration<Rep, Period>;

  using Unit = Literal<"nanoseconds", "microseconds", "milliseconds", "seconds",
                       "minutes", "hours", "days", "weeks", "months", "years">;

  using SupportedTypes =
      Variant<std::chrono::nanoseconds, std::chrono::microseconds,
              std::chrono::milliseconds, std::chrono::seconds,
              std::chrono::minutes, std::chrono::hours, std::chrono::days,
              std::chrono::weeks, std::chrono::months, std::chrono::years>;

  struct RType {
    int64_t count;
    Unit unit;
  };

  static Result<DurationType> read(const R& _r,
                                   const InputVarType& _var) noexcept {
    return Parser<R, W, RType, ProcessorsType>::read(_r, _var)
        .and_then(to_duration)
        .transform([](auto&& _duration) {
          return _duration.visit([](auto&& _d) -> DurationType {
            return std::chrono::duration_cast<DurationType>(std::move(_d));
          });
        });
  }

  template <class P>
  static void write(const W& _w, const DurationType& _d,
                    const P& _parent) noexcept {
    const auto r =
        RType{.count = static_cast<int64_t>(_d.count()), .unit = make_unit()};
    return Parser<R, W, RType, ProcessorsType>::write(_w, r, _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, RType, ProcessorsType>::to_schema(_definitions);
  }

 private:
  static Result<SupportedTypes> to_duration(const RType& _r) {
    switch (_r.unit.value()) {
      case Unit::value_of<"nanoseconds">():
        return SupportedTypes(std::chrono::nanoseconds(_r.count));

      case Unit::value_of<"microseconds">():
        return SupportedTypes(std::chrono::microseconds(_r.count));

      case Unit::value_of<"milliseconds">():
        return SupportedTypes(std::chrono::milliseconds(_r.count));

      case Unit::value_of<"seconds">():
        return SupportedTypes(std::chrono::seconds(_r.count));

      case Unit::value_of<"minutes">():
        return SupportedTypes(std::chrono::minutes(_r.count));

      case Unit::value_of<"days">():
        return SupportedTypes(std::chrono::days(_r.count));

      case Unit::value_of<"weeks">():
        return SupportedTypes(std::chrono::weeks(_r.count));

      case Unit::value_of<"months">():
        return SupportedTypes(std::chrono::months(_r.count));

      case Unit::value_of<"years">():
        return SupportedTypes(std::chrono::years(_r.count));

      default:
        return error("Unsupported unit.");
    }
  }

  static auto make_unit() noexcept {
    if constexpr (std::is_same_v<DurationType, std::chrono::nanoseconds>) {
      return Unit::make<"nanoseconds">();

    } else if constexpr (std::is_same_v<DurationType,
                                        std::chrono::microseconds>) {
      return Unit::make<"microseconds">();

    } else if constexpr (std::is_same_v<DurationType,
                                        std::chrono::milliseconds>) {
      return Unit::make<"milliseconds">();

    } else if constexpr (std::is_same_v<DurationType, std::chrono::seconds>) {
      return Unit::make<"seconds">();

    } else if constexpr (std::is_same_v<DurationType, std::chrono::minutes>) {
      return Unit::make<"minutes">();

    } else if constexpr (std::is_same_v<DurationType, std::chrono::hours>) {
      return Unit::make<"hours">();

    } else if constexpr (std::is_same_v<DurationType, std::chrono::days>) {
      return Unit::make<"days">();

    } else if constexpr (std::is_same_v<DurationType, std::chrono::weeks>) {
      return Unit::make<"weeks">();

    } else if constexpr (std::is_same_v<DurationType, std::chrono::months>) {
      return Unit::make<"months">();

    } else if constexpr (std::is_same_v<DurationType, std::chrono::years>) {
      return Unit::make<"years">();

    } else {
      static_assert(always_false_v<DurationType>, "Unsupported type.");
    }
  };
};

}  // namespace rfl::parsing

#endif
