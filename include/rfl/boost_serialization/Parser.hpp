#ifndef RFL_BOOST_SERIALIZATION_PARSER_HPP_
#define RFL_BOOST_SERIALIZATION_PARSER_HPP_

#include "../Generic.hpp"
#include "../NamedTuple.hpp"
#include "../Tuple.hpp"
#include "../always_false.hpp"
#include "../parsing/Parser.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl {
namespace parsing {

/// Boost.Serialization requires us to explicitly set all fields. Because
/// of that, we require all of the fields and then set them to nullptr, if
/// necessary.
template <class IArchive, class OArchive, class ProcessorsType,
          class... FieldTypes>
  requires AreReaderAndWriter<boost_serialization::Reader<IArchive>,
                              boost_serialization::Writer<OArchive>,
                              NamedTuple<FieldTypes...>>
struct Parser<boost_serialization::Reader<IArchive>,
              boost_serialization::Writer<OArchive>, NamedTuple<FieldTypes...>,
              ProcessorsType>
    : public NamedTupleParser<
          boost_serialization::Reader<IArchive>,
          boost_serialization::Writer<OArchive>,
          /*_ignore_empty_containers=*/false,
          /*_all_required=*/true,
          /*_no_field_names=*/ProcessorsType::no_field_names_, ProcessorsType,
          FieldTypes...> {};

template <class IArchive, class OArchive, class ProcessorsType, class... Ts>
  requires AreReaderAndWriter<boost_serialization::Reader<IArchive>,
                              boost_serialization::Writer<OArchive>,
                              rfl::Tuple<Ts...>>
struct Parser<boost_serialization::Reader<IArchive>,
              boost_serialization::Writer<OArchive>, rfl::Tuple<Ts...>,
              ProcessorsType>
    : public TupleParser<boost_serialization::Reader<IArchive>,
                         boost_serialization::Writer<OArchive>,
                         /*_ignore_empty_containers=*/false,
                         /*_all_required=*/true, ProcessorsType,
                         rfl::Tuple<Ts...>> {};

template <class IArchive, class OArchive, class ProcessorsType, class... Ts>
  requires AreReaderAndWriter<boost_serialization::Reader<IArchive>,
                              boost_serialization::Writer<OArchive>,
                              std::tuple<Ts...>>
struct Parser<boost_serialization::Reader<IArchive>,
              boost_serialization::Writer<OArchive>, std::tuple<Ts...>,
              ProcessorsType>
    : public TupleParser<boost_serialization::Reader<IArchive>,
                         boost_serialization::Writer<OArchive>,
                         /*_ignore_empty_containers=*/false,
                         /*_all_required=*/true, ProcessorsType,
                         std::tuple<Ts...>> {};

template <class IArchive, class OArchive, class T, bool _skip_serialization,
          bool _skip_deserialization, class ProcessorsType>
  requires AreReaderAndWriter<
      boost_serialization::Reader<IArchive>,
      boost_serialization::Writer<OArchive>,
      internal::Skip<T, _skip_serialization, _skip_deserialization>>
struct Parser<boost_serialization::Reader<IArchive>,
              boost_serialization::Writer<OArchive>,
              internal::Skip<T, _skip_serialization, _skip_deserialization>,
              ProcessorsType> {
  using R = boost_serialization::Reader<IArchive>;
  using W = boost_serialization::Writer<OArchive>;

  template <class U>
  static Result<internal::Skip<T, _skip_serialization, _skip_deserialization>>
  read(const R&, const U&) noexcept {
    static_assert(always_false_v<T>,
                  "rfl::Skip is unsupported in Boost.Serialization.");
    return error("Unsupported");
  }

  template <class P>
  static void write(const W& /*_w*/,
                    const internal::Skip<T, _skip_serialization,
                                         _skip_deserialization>& /*_skip*/,
                    const P& /*_parent*/) noexcept {
    static_assert(always_false_v<P>,
                  "rfl::Skip is unsupported in Boost.Serialization.");
  }

  template <class U>
  static schema::Type to_schema(U* /*_definitions*/) {
    static_assert(always_false_v<U>,
                  "rfl::Skip is unsupported in Boost.Serialization.");
    return schema::Type{};
  }
};

}  // namespace parsing
}  // namespace rfl

namespace rfl::boost_serialization {

template <class IArchive, class OArchive, class T, class ProcessorsType>
using Parser =
    parsing::Parser<Reader<IArchive>, Writer<OArchive>, T, ProcessorsType>;

}  // namespace rfl::boost_serialization

#endif
