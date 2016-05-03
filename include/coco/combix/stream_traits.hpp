#ifndef COCO_COMBIX_STREAM_HPP_
#define COCO_COMBIX_STREAM_HPP_

#include <iterator>
#include <type_traits>

#include <coco/expected.hpp>

namespace coco {
  namespace combix {

    template <typename T>
    struct parse_error;

    template <typename T>
    struct stream_traits {
      using value_type = typename T::value_type;
      using iterator_type = typename T::iterator_type;

      static T save(T const& t) {
        return t.save();
      }

      static void restore(T& t, T&& saved) {
        return t.restore(std::move(saved));
      }

      static coco::expected<value_type, parse_error<T>> uncons(T& t) {
        return t.uncons();
      }

      static bool is_eof(T const& t) {
        return t.is_eof();
      }
    };

    template <typename Stream>
    using stream_value_of_t = typename stream_traits<Stream>::value_type;

    template <typename T>
    coco::expected<typename stream_traits<T>::value_type,
                   parse_error<T>>
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

    template <typename T>
    bool is_eof(T const& t) {
      return stream_traits<T>::is_eof(t);
    }

  } // namespace combix
} // namespace coco

#endif
