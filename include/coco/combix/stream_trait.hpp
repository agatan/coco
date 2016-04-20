#ifndef COCO_COMBIX_STREAM_HPP_
#define COCO_COMBIX_STREAM_HPP_

#include <iterator>
#include <type_traits>

#include <coco/combix/error.hpp>
#include <coco/expected.hpp>

namespace coco {
  namespace combix {

    template <typename T>
    struct stream_trait {
      using value_type = typename T::value_type;

      static T save(T const& t) {
        return t.save();
      }

      static coco::expected<value_type, error<value_type>> peek(T& t) {
        return t.peek();
      }

      static coco::expected<value_type, error<value_type>> uncons(T& t) {
        return t.uncons();
      }
    };

    template <typename T>
    struct is_stream_trait : std::false_type {};

    template <typename T>
    constexpr bool is_stream_trait_v = is_stream_trait<T>::value;

    template <typename T,
              typename std::enable_if<is_stream_trait_v<T>,
                                      std::nullptr_t>::type = nullptr>
    coco::expected<typename stream_trait<T>::value_type,
                   error<typename stream_trait<T>::value_type>>
    peek(T& t) {
      return stream_trait<T>::peek(t);
    }

    template <typename T,
              typename std::enable_if<is_stream_trait_v<T>,
                                      std::nullptr_t>::type = nullptr>
    coco::expected<typename stream_trait<T>::value_type,
                   error<typename stream_trait<T>::value_type>>
    uncons(T& t) {
      return stream_trait<T>::uncons(t);
    }

    template <typename T, typename std::enable_if<is_stream_trait_v<T>,
                                                  std::nullptr_t>::type = nullptr>
    T save(T const& t) {
      return stream_trait<T>::save(t);
    }

  } // namespace combix
} // namespace coco

#endif
