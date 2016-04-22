#ifndef COCO_COMBIX_PAESE_RESULT_HPP
#define COCO_COMBIX_PAESE_RESULT_HPP

#include <coco/combix/error.hpp>
#include <coco/combix/stream_trait.hpp>
#include <coco/expected.hpp>

namespace coco {
  namespace combix {

    template <typename T, typename S>
    using parse_result =
        coco::expected<T, parse_error<typename stream_trait<S>::value_type>>;


  } // namespace combix
} // namespace coco

#endif
