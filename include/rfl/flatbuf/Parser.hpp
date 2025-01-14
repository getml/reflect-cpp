#ifndef RFL_FLATBUF_PARSER_HPP_
#define RFL_FLATBUF_PARSER_HPP_

#include "../Generic.hpp"
#include "../Tuple.hpp"
#include "../always_false.hpp"
#include "../parsing/Parser.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl {
namespace parsing {

/// Cap'n proto requires us to explicitly set all fields. Because
/// of that, we require all of the fields and then set them to nullptr, if
/// necessary.
template <class ProcessorsType, class... FieldTypes>
  requires AreReaderAndWriter<flatbuf::Reader, flatbuf::Writer,
                              NamedTuple<FieldTypes...>>
struct Parser<flatbuf::Reader, flatbuf::Writer, NamedTuple<FieldTypes...>,
              ProcessorsType>
    : public NamedTupleParser<
          flatbuf::Reader, flatbuf::Writer,
          /*_ignore_empty_containers=*/false,
          /*_all_required=*/true,
          /*_no_field_names=*/ProcessorsType::no_field_names_, ProcessorsType,
          FieldTypes...> {};

template <class ProcessorsType, class... Ts>
  requires AreReaderAndWriter<flatbuf::Reader, flatbuf::Writer,
                              rfl::Tuple<Ts...>>
struct Parser<flatbuf::Reader, flatbuf::Writer, rfl::Tuple<Ts...>,
              ProcessorsType>
    : public TupleParser<flatbuf::Reader, flatbuf::Writer,
                         /*_ignore_empty_containers=*/false,
                         /*_all_required=*/true, ProcessorsType,
                         rfl::Tuple<Ts...>> {};

template <class ProcessorsType, class... Ts>
  requires AreReaderAndWriter<flatbuf::Reader, flatbuf::Writer,
                              std::tuple<Ts...>>
struct Parser<flatbuf::Reader, flatbuf::Writer, std::tuple<Ts...>,
              ProcessorsType>
    : public TupleParser<flatbuf::Reader, flatbuf::Writer,
                         /*_ignore_empty_containers=*/false,
                         /*_all_required=*/true, ProcessorsType,
                         std::tuple<Ts...>> {};

template <class ProcessorsType>
  requires AreReaderAndWriter<flatbuf::Reader, flatbuf::Writer, Generic>
struct Parser<flatbuf::Reader, flatbuf::Writer, Generic, ProcessorsType> {
  template <class T>
  static Result<Generic> read(const flatbuf::Reader&, const T&) noexcept {
    static_assert(always_false_v<T>,
                  "Generics are unsupported in Cap'n Proto.");
    return Error("Unsupported");
  }

  template <class P>
  static void write(const flatbuf::Writer&, const Generic&,
                    const P&) noexcept {
    static_assert(always_false_v<P>,
                  "Generics are unsupported in Cap'n Proto.");
  }

  template <class T>
  static schema::Type to_schema(T*) {
    static_assert(always_false_v<T>,
                  "Generics are unsupported in Cap'n Proto.");
    return schema::Type{};
  }
};

template <class T, bool _skip_serialization, bool _skip_deserialization,
          class ProcessorsType>
  requires AreReaderAndWriter<
      flatbuf::Reader, flatbuf::Writer,
      internal::Skip<T, _skip_serialization, _skip_deserialization>>
struct Parser<flatbuf::Reader, flatbuf::Writer,
              internal::Skip<T, _skip_serialization, _skip_deserialization>,
              ProcessorsType> {
  using R = flatbuf::Reader;
  using W = flatbuf::Writer;

  template <class U>
  static Result<internal::Skip<T, _skip_serialization, _skip_deserialization>>
  read(const R&, const U&) noexcept {
    static_assert(always_false_v<T>,
                  "rfl::Skip is unsupported in Cap'n Proto.");
    return Error("Unsupported");
  }

  template <class P>
  static void write(const W& _w,
                    const internal::Skip<T, _skip_serialization,
                                         _skip_deserialization>& _skip,
                    const P& _parent) noexcept {
    static_assert(always_false_v<P>,
                  "rfl::Skip is unsupported in Cap'n Proto.");
  }

  template <class U>
  static schema::Type to_schema(U* _definitions) {
    static_assert(always_false_v<U>,
                  "rfl::Skip is unsupported in Cap'n Proto.");
    return schema::Type{};
  }
};

}  // namespace parsing
}  // namespace rfl

namespace rfl {
namespace flatbuf {

template <class T, class ProcessorsType>
using Parser = parsing::Parser<Reader, Writer, T, ProcessorsType>;

}
}  // namespace rfl

#endif