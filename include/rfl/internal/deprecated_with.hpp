#ifndef RFL_INTERNAL_DEPRECATED_WITH_HPP_
#define RFL_INTERNAL_DEPRECATED_WITH_HPP_

/// Marks a legacy with_<field>() method as deprecated, pointing at the
/// unified RFL_SETTINGS_OPS-generated with<&T::field>() / with<"field">()
/// API. Used in csv::Settings and parquet::Settings to keep the old
/// per-field setter names working for existing users.
#define RFL_DEPRECATED_WITH(field_name)                                     \
  [[deprecated("Use .with<&Settings::" #field_name ">(value) or "           \
               ".with<\"" #field_name "\">(value) instead.")]]

#endif
