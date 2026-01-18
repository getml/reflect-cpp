#ifndef RFL_ATOMIC_ISATOMIC_HPP_
#define RFL_ATOMIC_ISATOMIC_HPP_

#include <array>
#include <atomic>
#include <memory>
#include <type_traits>

#include "../Box.hpp"
#include "../NamedTuple.hpp"
#include "../Ref.hpp"
#include "../Tuple.hpp"
#include "../named_tuple_t.hpp"
#include "../to_view.hpp"

namespace rfl::atomic {

template <class T>
struct is_atomic;

template <class T>
struct is_atomic {
  static constexpr bool value = false;
  using RemoveAtomicT = T;
  static void set(RemoveAtomicT&& val, T* _t) { *_t = std::forward<T>(val); };
};

template <class T>
struct is_atomic<std::atomic<T>> {
  static constexpr bool value = true;
  using RemoveAtomicT = T;
  static void set(RemoveAtomicT&& val, std::atomic<T>* _t) {
    _t->store(std::forward<RemoveAtomicT>(val), std::memory_order_relaxed);
  };
};

template <class T, size_t N>
struct is_atomic<std::array<T, N>> {
  using Type = std::remove_cvref_t<T>;

  static constexpr bool value = is_atomic<Type>::value;
  using RemoveAtomicT = std::array<typename is_atomic<Type>::RemoveAtomicT, N>;
  static void set(RemoveAtomicT&& val, std::array<T, N>* _t) {
    for (size_t i = 0; i < N; ++i) {
      is_atomic<T>::set(
          std::forward<typename is_atomic<Type>::RemoveAtomicT>(val[i]),
          &((*_t)[i]));
    }
  }
};

template <class T, size_t N>
struct is_atomic<T[N]> {
  using Type = std::remove_cvref_t<T>;

  static constexpr bool value = is_atomic<Type>::value;
  using RemoveAtomicT = std::array<typename is_atomic<Type>::RemoveAtomicT, N>;
  static void set(RemoveAtomicT&& val, T (*_t)[N]) {
    for (size_t i = 0; i < N; ++i) {
      is_atomic<T>::set(
          std::forward<typename is_atomic<T>::RemoveAtomicT>(val[i]),
          &((*_t)[i]));
    }
  }
};

template <class T>
struct is_atomic<std::shared_ptr<T>> {
  using Type = std::remove_cvref_t<T>;

  static constexpr bool value = is_atomic<Type>::value;
  using RemoveAtomicT =
      std::shared_ptr<typename is_atomic<Type>::RemoveAtomicT>;
  static void set(RemoveAtomicT&& val, std::shared_ptr<T>* _t) {
    if (val) {
      auto ptr = std::make_shared<Type>();
      is_atomic<T>::set(
          std::forward<typename is_atomic<T>::RemoveAtomicT>(*val), ptr.get());
      *_t = std::move(ptr);
    }
  }
};

template <class T>
struct is_atomic<std::unique_ptr<T>> {
  using Type = std::remove_cvref_t<T>;

  static constexpr bool value = is_atomic<Type>::value;
  using RemoveAtomicT =
      std::unique_ptr<typename is_atomic<Type>::RemoveAtomicT>;
  static void set(RemoveAtomicT&& val, std::unique_ptr<T>* _t) {
    if (val) {
      auto ptr = std::make_unique<Type>();
      is_atomic<T>::set(
          std::forward<typename is_atomic<T>::RemoveAtomicT>(*val), ptr.get());
      *_t = std::move(ptr);
    }
  }
};

template <class T>
struct is_atomic<Ref<T>> {
  using Type = std::remove_cvref_t<T>;

  static constexpr bool value = is_atomic<Type>::value;
  using RemoveAtomicT = Ref<typename is_atomic<Type>::RemoveAtomicT>;
  static void set(RemoveAtomicT&& val, Ref<T>* _t) {
    auto ref = Ref<Type>::make();
    is_atomic<Type>::set(
        std::forward<typename is_atomic<Type>::RemoveAtomicT>(*val), &(*ref));
    *_t = std::move(ref);
  }
};

template <class T>
struct is_atomic<Box<T>> {
  using Type = std::remove_cvref_t<T>;
  static constexpr bool value = is_atomic<Type>::value;
  using RemoveAtomicT = Box<typename is_atomic<Type>::RemoveAtomicT>;
  static void set(RemoveAtomicT&& val, Box<T>* _t) {
    auto box = Box<Type>::make();
    is_atomic<Type>::set(
        std::forward<typename is_atomic<Type>::RemoveAtomicT>(*val), &(*box));
    *_t = std::move(box);
  }
};

template <class... Fields>
struct is_atomic<NamedTuple<Fields...>> {
  static constexpr bool value =
      (is_atomic<typename Fields::Type>::value || ...);

  using RemoveAtomicT = NamedTuple<
      rfl::Field<Fields::name_,
                 typename is_atomic<typename Fields::Type>::RemoveAtomicT>...>;

  static void set(RemoveAtomicT&& val, NamedTuple<Fields...>* _t) {
    (is_atomic<typename Fields::Type>::set(
         std::forward<typename is_atomic<typename Fields::Type>::RemoveAtomicT>(
             val.template get<Fields::name_>()),
         &(_t->template get<Fields::name_>())),
     ...);
  }
};

template <class T>
  requires(std::is_class_v<T> && std::is_aggregate_v<T> &&
           !std::is_move_constructible_v<T>)
struct is_atomic<T> {
  static constexpr bool value = is_atomic<named_tuple_t<T>>::value;

  using RemoveAtomicT = typename is_atomic<named_tuple_t<T>>::RemoveAtomicT;

  static void set(RemoveAtomicT&& val, T* _t) {
    using Fields = typename named_tuple_t<T>::Fields;

    const auto view = to_view(*_t);

    const auto set_field = [&]<size_t _i>(std::integral_constant<size_t, _i>) {
      using FieldType = typename rfl::tuple_element_t<_i, Fields>::Type;
      using FieldRemoveAtomicT =
          typename is_atomic<std::remove_cvref_t<FieldType>>::RemoveAtomicT;

      is_atomic<std::remove_cvref_t<FieldType>>::set(
          std::forward<FieldRemoveAtomicT>(val.template get<_i>()),
          view.template get<_i>());
    };

    constexpr size_t num_fields = std::remove_cvref_t<decltype(view)>::size();

    [&]<size_t... _is>(std::index_sequence<_is...>) {
      (set_field(std::integral_constant<size_t, _is>{}), ...);
    }(std::make_index_sequence<num_fields>{});
  }
};

template <class T>
  requires(std::is_class_v<T> && std::is_aggregate_v<T> &&
           std::is_move_constructible_v<T>)
struct is_atomic<T> {
  static constexpr bool value = false;
  using RemoveAtomicT = T;
  static void set(RemoveAtomicT&& val, T* _t) { *_t = std::forward<T>(val); };
};

template <class T>
constexpr bool is_atomic_v = is_atomic<std::remove_cvref_t<T>>::value;

}  // namespace rfl::atomic

#endif
