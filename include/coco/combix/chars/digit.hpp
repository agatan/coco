#ifndef COCO_COMBIX_CHARS_DIGIT_HPP_
#define COCO_COMBIX_CHARS_DIGIT_HPP_

#include <cctype>
#include <type_traits>

#include <coco/combix/primitives.hpp>
#include <coco/combix/parse_result.hpp>
#include <coco/combix/parser_traits.hpp>
#include <coco/combix/combinators/expected.hpp>
#include <coco/combix/primitives.hpp>

namespace coco {
  namespace combix {

    using digit_parser = expected_parser<satisfy_parser<bool(*)(char)>>;

    digit_parser digit();

  } // namespace combix
} // namespace coco

#endif
