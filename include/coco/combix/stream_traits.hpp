#ifndef COCO_COMBIX_STREAM_HPP_
#define COCO_COMBIX_STREAM_HPP_

#include <iterator>
#include <type_traits>

#include <coco/combix/error.hpp>
#include <coco/expected.hpp>

namespace coco {
  namespace combix {

    template <typename T>
    struct stream_traits {
      using value_type = typename T::value_type;

      static T save(T const& t) {
        return t.save();
      }

      static void restore(T& t, T&& saved) {
        return t.restore(std::move(saved));
      }

      static coco::expected<value_type, parse_error<value_type>> peek(T& t) {
        return t.peek();
      }

      static coco::expected<value_type, parse_error<value_type>> uncons(T& t) {
        return t.uncons();
      }
    };

    template <typename T>
    coco::expected<typename stream_traits<T>::value_type,
                   parse_error<typename stream_traits<T>::value_type>>
    peek(T& t) {
      return stream_traits<T>::peek(t);
    }

    template <typename T>
    coco::expected<typename stream_traits<T>::value_type,
                   parse_error<typename stream_traits<T>::value_type>>
    uncons(T& t) {
      return stream_traits<T>::uncons(t);
    }

    template <typename T>
    T save(T const& t) {
      return stream_traits<T>::save(t);
    }

    template <typename T>
    void restore(T& t, T&& saved) {
      stream_traits<T>::restore(t, std::move(saved));
    }

  } // namespace combix
} // namespace coco

#endif
