#ifndef COCO_EXPECTED_HPP_
#define COCO_EXPECTED_HPP_

#include <type_traits>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

namespace coco {

  namespace detail {

    template<typename T>
    struct error_holder {
      static_assert(std::is_default_constructible<T>::value,
                    "error type should be default constructible");
      T error;

      error_holder() = default;
      ~error_holder() = default;

      error_holder(T const& error): error(error) {}
      error_holder(T&& error): error(std::move(error)) {}

      error_holder(error_holder const& e): error(e.error) {}
      error_holder(error_holder&& e): error(std::move(e.error)) {}

      error_holder& operator=(error_holder const& e) {
        error = e.error;
        return *this;
      }
      error_holder& operator=(error_holder&& e) {
        error = std::move(e.error);
        return *this;
      }
    };

  } // namespace detail

  template <typename E>
  detail::error_holder<typename std::decay<E>::type>
  make_error(E&& e) {
    return detail::error_holder<typename std::decay<E>::type>{e};
  }

  template<typename T, typename E>
  struct expected {
    // E should be default constructible (checked in detail::error_holder)
    expected() = default;
    ~expected() = default;

    // constructors
    expected(T const& v): value(v) {}
    expected(T&& v): value(std::move(v)) {}

    expected(E const& e): value(detail::error_holder<E>{e}) {}
    expected(E&& e): value(detail::error_holder<E>{std::move(e)}) {}

    expected(detail::error_holder<E> const& e): value(e) {}
    expected(detail::error_holder<E>&& e): value(std::move(e)) {}

    expected(expected const& exp): value(exp.value) {}
    expected(expected&& exp): value(std::move(exp.value)) {}

    expected& operator=(expected const& e) {
      value = e.value;
      return *this;
    }
    expected& operator=(expected&& e) {
      value = std::move(e.value);
      return *this;
    }

    bool is_ok() const noexcept {
      return value.which() == 1;
    }

    bool is_err() const noexcept {
      return value.which() == 0;
    }

    operator bool() const noexcept {
      return is_ok();
    }


    // extraction
    boost::optional<T const&> get() const noexcept {
      if (is_ok()) {
        return boost::get<T const&>(value);
      }
      return boost::none;
    }
    boost::optional<T&> get() noexcept {
      if (is_ok()) {
        return boost::get<T&>(value);
      }
      return boost::none;
    }

    T const& get_exn() const {
      return boost::get<T const&>(value);
    }
    T& get_exn() {
      return boost::get<T&>(value);
    }

    boost::optional<E const&> get_err() const noexcept {
      if (is_err()) {
        return boost::get<detail::error_holder<E> const&>(value).error;
      }
      return boost::none;
    }
    boost::optional<E&> get_err() noexcept {
      if (is_err()) {
        return boost::get<detail::error_holder<E>&>(value).error;
      }
      return boost::none;
    }

    E const& get_err_exn() const {
      return boost::get<detail::error_holder<E> const&>(value).error;
    }
    E& get_err_exn() {
      return boost::get<detail::error_holder<E>&>(value).error;
    }


    // combinators
    template <typename F>
    expected<typename std::result_of<F(T)>::type, E>
    map(F&& f) const &&;

    template <typename F>
    expected<T, typename std::result_of<F(E)>::type>
    map_err(F&& f) const &&;

    template <typename F>
    typename std::result_of<F(T)>::type
    and_then(F&& f) const &&;

    template <typename E2>
    expected<T, E2> or_(expected<T, E2> const& e) const {
      if (is_ok()) {
        return get_exn();
      }
      return e;
    }
    template <typename E2>
    expected<T, E2> or_(expected<T, E2> && e) const {
      if (is_ok()) {
        return get_exn();
      }
      return e;
    }

    T const& get_or_else(T const& v) const & {
      if (is_ok()) {
        return get_exn();
      }
      return v;
    }
    T get_or_else(T&& v) const & {
      if (is_ok()) {
        return get_exn();
      }
      return v;
    }
    T get_or_else(T const& v) && {
      if (is_ok()) {
        return std::move(get_exn());
      }
      return v;
    }
    T get_or_else(T&& v) && {
      if (is_ok()) {
        return std::move(get_exn());
      }
      return v;
    }

  private:
    using value_type = boost::variant<detail::error_holder<E>, T>;
    value_type value;
  };

  // implementations
  // combinators
  template <typename T, typename E>
  template <typename F>
  expected<typename std::result_of<F(T)>::type, E>
  expected<T, E>::map(F&& f) const && {
    if (auto v = get()) {
      return f(std::move(*v));
    }
    return get_err_exn();
  }

  template <typename T, typename E>
  template <typename F>
  expected<T, typename std::result_of<F(E)>::type>
  expected<T, E>::map_err(F&& f) const && {
    if (auto e = get_err()) {
      return f(std::move(*e));
    }
    return get_exn();
  }

  template <typename T, typename E>
  template <typename F>
  typename std::result_of<F(T)>::type
  expected<T, E>::and_then(F&& f) const && {
    if (auto e = get_err()) {
      return *e;
    }
    return f(get_exn());
  }

} // namespace coco

#endif
