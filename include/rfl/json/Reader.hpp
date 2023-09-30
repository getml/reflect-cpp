#ifndef RFL_JSON_READER_HPP_
#define RFL_JSON_READER_HPP_

#include <yyjson.h>

#include <exception>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "rfl/Result.hpp"
#include "rfl/always_false.hpp"

namespace rfl {
namespace json {

struct YYJSONOutputArray {
    yyjson_mut_val* val_;
};

struct YYJSONOutputObject {
    yyjson_mut_val* val_;
};

struct YYJSONOutputVar {
    YYJSONOutputVar(yyjson_mut_val* _val) : val_(_val) {}

    YYJSONOutputVar(YYJSONOutputArray _arr) : val_(_arr.val_) {}

    YYJSONOutputVar(YYJSONOutputObject _obj) : val_(_obj.val_) {}

    yyjson_mut_val* val_;
};

struct Reader {
    struct YYJSONInputArray {
        yyjson_val* val_;
    };

    struct YYJSONInputObject {
        yyjson_val* val_;
    };

    struct YYJSONInputVar {
        yyjson_val* val_;
    };

    using InputArrayType = YYJSONInputArray;
    using InputObjectType = YYJSONInputObject;
    using InputVarType = YYJSONInputVar;

    template <class T, class = void>
    struct has_from_json_obj : std::false_type {};

    template <class T>
    struct has_from_json_obj<T, std::enable_if_t<std::is_invocable_r<
                                    T, decltype(T::from_json_obj),
                                    typename Reader::InputVarType>::value>>
        : std::true_type {};

    template <class T>
    static constexpr bool has_custom_constructor = has_from_json_obj<T>::value;

    rfl::Result<InputVarType> get_field(const std::string& _name,
                                        InputObjectType* _obj) const noexcept {
        const auto var =
            InputVarType(yyjson_obj_get(_obj->val_, _name.c_str()));
        if (!var.val_) {
            return rfl::Error("Object contains no field named '" + _name +
                              "'.");
        }
        return var;
    }

    bool is_empty(InputVarType* _var) const noexcept {
        return !_var->val_ || yyjson_is_null(_var->val_);
    }

    template <class T>
    rfl::Result<T> to_basic_type(InputVarType* _var) const noexcept {
        if constexpr (std::is_same<std::decay_t<T>, std::string>()) {
            const auto r = yyjson_get_str(_var->val_);
            if (r == NULL) {
                return rfl::Error("Could not cast to string.");
            }
            return std::string(r);
        } else if constexpr (std::is_same<std::decay_t<T>, bool>()) {
            if (!yyjson_is_bool(_var->val_)) {
                return rfl::Error("Could not cast to boolean.");
            }
            return yyjson_get_bool(_var->val_);
        } else if constexpr (std::is_floating_point<std::decay_t<T>>()) {
            if (!yyjson_is_num(_var->val_)) {
                return rfl::Error("Could not cast to double.");
            }
            return static_cast<T>(yyjson_get_num(_var->val_));
        } else if constexpr (std::is_integral<std::decay_t<T>>()) {
            if (!yyjson_is_int(_var->val_)) {
                return rfl::Error("Could not cast to int.");
            }
            return static_cast<T>(yyjson_get_int(_var->val_));
        } else {
            static_assert(rfl::always_false_v<T>, "Unsupported type.");
        }
    }

    rfl::Result<InputArrayType> to_array(InputVarType* _var) const noexcept {
        if (!yyjson_is_arr(_var->val_)) {
            return rfl::Error("Could not cast to array!");
        }
        return InputArrayType(_var->val_);
    }

    std::map<std::string, InputVarType> to_map(
        InputObjectType* _obj) const noexcept {
        std::map<std::string, InputVarType> m;
        yyjson_obj_iter iter;
        yyjson_obj_iter_init(_obj->val_, &iter);
        yyjson_val* key;
        while ((key = yyjson_obj_iter_next(&iter))) {
            const auto p =
                std::make_pair(yyjson_get_str(key),
                               InputVarType(yyjson_obj_iter_get_val(key)));
            m.emplace(std::move(p));
        }
        return m;
    }

    rfl::Result<InputObjectType> to_object(InputVarType* _var) const noexcept {
        if (!yyjson_is_obj(_var->val_)) {
            return rfl::Error("Could not cast to object!");
        }
        return InputObjectType(_var->val_);
    }

    std::vector<InputVarType> to_vec(InputArrayType* _arr) const noexcept {
        std::vector<InputVarType> vec;
        yyjson_val* val;
        yyjson_arr_iter iter;
        yyjson_arr_iter_init(_arr->val_, &iter);
        while ((val = yyjson_arr_iter_next(&iter))) {
            vec.push_back(InputVarType(val));
        }
        return vec;
    }

    template <class T>
    rfl::Result<T> use_custom_constructor(InputVarType* _var) const noexcept {
        try {
            return T::from_json_obj(*_var);
        } catch (std::exception& e) {
            return rfl::Error(e.what());
        }
    }
};

}  // namespace json
}  // namespace rfl

#endif  // JSON_PARSER_HPP_
