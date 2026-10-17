#ifndef RFL_JSON_YYJSON_HELPERS_HPP_
#define RFL_JSON_YYJSON_HELPERS_HPP_

#if __has_include(<yyjson.h>)
#include <yyjson.h>
#else
#include "../thirdparty/yyjson.h"
#endif

#include "../common.hpp"

namespace rfl::json::yyjson_detail {

inline constexpr yyjson_read_flag read_insitu = YYJSON_READ_INSITU;

// Module importers call external wrappers instead of yyjson static functions.
#ifdef REFLECTCPP_BUILD_MODULES
RFL_API yyjson_val *yyjson_arr_get(yyjson_val *arr, size_t idx);
RFL_API bool yyjson_arr_iter_init(yyjson_val *arr, yyjson_arr_iter *iter);
RFL_API yyjson_val *yyjson_arr_iter_next(yyjson_arr_iter *iter);
RFL_API void yyjson_doc_free(yyjson_doc *doc);
RFL_API yyjson_val *yyjson_doc_get_root(yyjson_doc *doc);
RFL_API bool yyjson_get_bool(yyjson_val *val);
RFL_API size_t yyjson_get_len(yyjson_val *val);
RFL_API double yyjson_get_num(yyjson_val *val);
RFL_API int64_t yyjson_get_sint(yyjson_val *val);
RFL_API const char *yyjson_get_str(yyjson_val *val);
RFL_API uint64_t yyjson_get_uint(yyjson_val *val);
RFL_API bool yyjson_is_arr(yyjson_val *val);
RFL_API bool yyjson_is_bool(yyjson_val *val);
RFL_API bool yyjson_is_int(yyjson_val *val);
RFL_API bool yyjson_is_null(yyjson_val *val);
RFL_API bool yyjson_is_num(yyjson_val *val);
RFL_API bool yyjson_is_obj(yyjson_val *val);
RFL_API bool yyjson_is_uint(yyjson_val *val);
RFL_API bool yyjson_mut_arr_add_val(yyjson_mut_val *arr, yyjson_mut_val *val);
RFL_API yyjson_mut_val *yyjson_mut_bool(yyjson_mut_doc *doc, bool val);
RFL_API void yyjson_mut_doc_set_root(yyjson_mut_doc *doc, yyjson_mut_val *root);
RFL_API yyjson_mut_val *yyjson_mut_int(yyjson_mut_doc *doc, int64_t num);
RFL_API bool yyjson_mut_obj_add(yyjson_mut_val *obj, yyjson_mut_val *key, yyjson_mut_val *val);
RFL_API yyjson_mut_val *yyjson_mut_real(yyjson_mut_doc *doc, double num);
RFL_API yyjson_mut_val *yyjson_mut_strncpy(yyjson_mut_doc *doc, const char *str, size_t len);
RFL_API yyjson_mut_val *yyjson_mut_uint(yyjson_mut_doc *doc, uint64_t num);
RFL_API yyjson_val *yyjson_obj_get(yyjson_val *obj, const char *key);
RFL_API yyjson_val *yyjson_obj_iter_get_val(yyjson_val *key);
RFL_API bool yyjson_obj_iter_init(yyjson_val *obj, yyjson_obj_iter *iter);
RFL_API yyjson_val *yyjson_obj_iter_next(yyjson_obj_iter *iter);
#else
using ::yyjson_arr_get;
using ::yyjson_arr_iter_init;
using ::yyjson_arr_iter_next;
using ::yyjson_doc_free;
using ::yyjson_doc_get_root;
using ::yyjson_get_bool;
using ::yyjson_get_len;
using ::yyjson_get_num;
using ::yyjson_get_sint;
using ::yyjson_get_str;
using ::yyjson_get_uint;
using ::yyjson_is_arr;
using ::yyjson_is_bool;
using ::yyjson_is_int;
using ::yyjson_is_null;
using ::yyjson_is_num;
using ::yyjson_is_obj;
using ::yyjson_is_uint;
using ::yyjson_mut_arr_add_val;
using ::yyjson_mut_bool;
using ::yyjson_mut_doc_set_root;
using ::yyjson_mut_int;
using ::yyjson_mut_obj_add;
using ::yyjson_mut_real;
using ::yyjson_mut_strncpy;
using ::yyjson_mut_uint;
using ::yyjson_obj_get;
using ::yyjson_obj_iter_get_val;
using ::yyjson_obj_iter_init;
using ::yyjson_obj_iter_next;
#endif

}  // namespace rfl::json::yyjson_detail

#endif
