"""
Creates the boilerplate code in rfl/internal/bind_to_tuple.hpp
"""


def _make_macro(num: int) -> str:
    fields = ", ".join((f"f{i+1}" for i in range(num)))
    return f"RFL_INTERNAL_TUPLE_VIEW_IF_YOU_SEE_AN_ERROR_REFER_TO_DOCUMENTATION_ON_C_ARRAYS({num}, {fields});"


def _generate_macros(minimum: int, maximum: int, depth: int = 0) -> str:
    if maximum <= minimum:
        return _make_macro(minimum)
    mid = int((minimum + maximum + 1) / 2)
    leg1 = _generate_macros(minimum, mid - 1, depth + 1)
    leg2 = _generate_macros(mid, maximum, depth + 1)
    return (
        "if constexpr(_num_fields < "
        + str(mid)
        + ") {\n"
        + "   " * (depth + 1)
        + leg1
        + "\n"
        + "   " * depth
        + "} else {\n"
        + "   " * (depth + 1)
        + leg2
        + "\n"
        + "   " * depth
        + "}"
    )


def _generate_bind_to_tuple_impl(maximum: int) -> str:
    macros = _generate_macros(1, maximum)
    return (
        """
template<class T, std::size_t _num_fields>
auto bind_to_tuple_impl(auto& _t) {
    if constexpr(_num_fields == 0) {
        return rfl::make_tuple();
    } else {
"""
        + macros
        + """
    }
}
    """
    )


with open("generated_code_bind_to_tuple.cpp", "w", encoding="utf-8") as codefile:
    codefile.write(_generate_bind_to_tuple_impl(256))
