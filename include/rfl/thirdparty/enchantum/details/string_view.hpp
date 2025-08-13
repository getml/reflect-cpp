#pragma once


#ifdef ENCHANTUM_CONFIG_FILE
  #include ENCHANTUM_CONFIG_FILE
#endif

#ifndef ENCHANTUM_ALIAS_STRING_VIEW
  #include <string_view>
#endif


namespace enchantum {
#ifdef ENCHANTUM_ALIAS_STRING_VIEW
ENCHANTUM_ALIAS_STRING_VIEW;
#else
using ::std::string_view;
#endif

} // namespace enchantum