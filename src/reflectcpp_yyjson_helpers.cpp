#include "rfl/json/yyjson_helpers.hpp"

namespace rfl::json::yyjson_detail {

yyjson_val *yyjson_arr_get(yyjson_val *arr, size_t idx) {
  return ::yyjson_arr_get(arr, idx);
}

bool yyjson_arr_iter_init(yyjson_val *arr, yyjson_arr_iter *iter) {
  return ::yyjson_arr_iter_init(arr, iter);
}

yyjson_val *yyjson_arr_iter_next(yyjson_arr_iter *iter) {
  return ::yyjson_arr_iter_next(iter);
}

void yyjson_doc_free(yyjson_doc *doc) {
  return ::yyjson_doc_free(doc);
}

yyjson_val *yyjson_doc_get_root(yyjson_doc *doc) {
  return ::yyjson_doc_get_root(doc);
}

bool yyjson_get_bool(yyjson_val *val) {
  return ::yyjson_get_bool(val);
}

size_t yyjson_get_len(yyjson_val *val) {
  return ::yyjson_get_len(val);
}

double yyjson_get_num(yyjson_val *val) {
  return ::yyjson_get_num(val);
}

int64_t yyjson_get_sint(yyjson_val *val) {
  return ::yyjson_get_sint(val);
}

const char *yyjson_get_str(yyjson_val *val) {
  return ::yyjson_get_str(val);
}

uint64_t yyjson_get_uint(yyjson_val *val) {
  return ::yyjson_get_uint(val);
}

bool yyjson_is_arr(yyjson_val *val) {
  return ::yyjson_is_arr(val);
}

bool yyjson_is_bool(yyjson_val *val) {
  return ::yyjson_is_bool(val);
}

bool yyjson_is_int(yyjson_val *val) {
  return ::yyjson_is_int(val);
}

bool yyjson_is_null(yyjson_val *val) {
  return ::yyjson_is_null(val);
}

bool yyjson_is_num(yyjson_val *val) {
  return ::yyjson_is_num(val);
}

bool yyjson_is_obj(yyjson_val *val) {
  return ::yyjson_is_obj(val);
}

bool yyjson_is_uint(yyjson_val *val) {
  return ::yyjson_is_uint(val);
}

bool yyjson_mut_arr_add_val(yyjson_mut_val *arr, yyjson_mut_val *val) {
  return ::yyjson_mut_arr_add_val(arr, val);
}

yyjson_mut_val *yyjson_mut_bool(yyjson_mut_doc *doc, bool val) {
  return ::yyjson_mut_bool(doc, val);
}

void yyjson_mut_doc_set_root(yyjson_mut_doc *doc, yyjson_mut_val *root) {
  return ::yyjson_mut_doc_set_root(doc, root);
}

yyjson_mut_val *yyjson_mut_int(yyjson_mut_doc *doc, int64_t num) {
  return ::yyjson_mut_int(doc, num);
}

bool yyjson_mut_obj_add(yyjson_mut_val *obj, yyjson_mut_val *key, yyjson_mut_val *val) {
  return ::yyjson_mut_obj_add(obj, key, val);
}

yyjson_mut_val *yyjson_mut_real(yyjson_mut_doc *doc, double num) {
  return ::yyjson_mut_real(doc, num);
}

yyjson_mut_val *yyjson_mut_strncpy(yyjson_mut_doc *doc, const char *str, size_t len) {
  return ::yyjson_mut_strncpy(doc, str, len);
}

yyjson_mut_val *yyjson_mut_uint(yyjson_mut_doc *doc, uint64_t num) {
  return ::yyjson_mut_uint(doc, num);
}

yyjson_val *yyjson_obj_get(yyjson_val *obj, const char *key) {
  return ::yyjson_obj_get(obj, key);
}

yyjson_val *yyjson_obj_iter_get_val(yyjson_val *key) {
  return ::yyjson_obj_iter_get_val(key);
}

bool yyjson_obj_iter_init(yyjson_val *obj, yyjson_obj_iter *iter) {
  return ::yyjson_obj_iter_init(obj, iter);
}

yyjson_val *yyjson_obj_iter_next(yyjson_obj_iter *iter) {
  return ::yyjson_obj_iter_next(iter);
}

}  // namespace rfl::json::yyjson_detail
