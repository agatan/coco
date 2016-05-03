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

    inline bool isdigit(char c) {
      return static_cast<bool>(std::isdigit(c));
    }

    inline digit_parser digit() {
      return expected(satisfy(isdigit), "digit");
    }

  } // namespace combix
} // namespace coco

#endif
