#ifndef COCO_COMBIX_PAESE_RESULT_HPP
#define COCO_COMBIX_PAESE_RESULT_HPP

#include <type_traits>

#include <coco/combix/error.hpp>
#include <coco/combix/stream_traits.hpp>
#include <coco/expected.hpp>

namespace coco {
  namespace combix {

    template <typename T, typename S>
    using parse_result =
        coco::expected<T, parse_error<S>>;

    template <typename>
    struct is_parse_result : std::false_type {};

    template <typename V, typename E>
    struct is_parse_result<coco::expected<V, E>> : std::true_type {};

  } // namespace combix
} // namespace coco

#endif
