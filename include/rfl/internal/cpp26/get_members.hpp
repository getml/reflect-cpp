#ifndef RFL_INTERNAL_CPP26_GETMEMBERS_HPP_
#define RFL_INTERNAL_CPP26_GETMEMBERS_HPP_

#include <meta>
#include <type_traits>

namespace rfl::internal::cpp26 {

template <class Type>
consteval std::vector<std::meta::info> get_members() {
  constexpr auto ctx = std::meta::access_context::current();

  std::vector<std::meta::info> members;

  constexpr auto bases =
      std::define_static_array(std::meta::bases_of(^^Type, ctx));

  const auto add_base_members =
      [&]<size_t _i>(std::integral_constant<size_t, _i>) {
        using BaseType = [:std::meta::type_of(bases[_i]):];
        constexpr auto base_members =
            std::define_static_array(get_members<BaseType>());
        template for (const size_t j :
                      std::make_index_sequence<base_members.size()>()) {
          members.push_back(base_members[j]);
        };
      };

  [&]<size_t... _is>(std::index_sequence<_is...>) {
    (add_base_members(std::integral_constant<size_t, _is>()), ...);
  }(std::make_index_sequence<bases.size()>());

  constexpr auto nonstatic_members = std::define_static_array(
      std::meta::nonstatic_data_members_of(^^Type, ctx));

  template for (const size_t i :
                std::make_index_sequence<nonstatic_members.size()>()) {
    members.push_back(nonstatic_members[i]);
  };

  return members;
}

}  // namespace rfl::internal::cpp26

#endif
