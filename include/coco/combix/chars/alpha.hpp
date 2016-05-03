#ifndef COCO_COMBIX_CHARS_ALPHA_HPP_
#define COCO_COMBIX_CHARS_ALPHA_HPP_

#include <cctype>
#include <type_traits>

#include <coco/combix/primitives.hpp>
#include <coco/combix/parse_result.hpp>
#include <coco/combix/parser_traits.hpp>
#include <coco/combix/combinators/expected.hpp>
#include <coco/combix/primitives.hpp>

namespace coco {
  namespace combix {

    using alpha_parser = expected_parser<satisfy_parser<bool(*)(char)>>;

    inline bool isalpha(char c) {
      return static_cast<bool>(std::isalpha(c));
    }

    inline alpha_parser alpha() {
      return expected(satisfy(isalpha), "alphabet");
    }

  } // namespace combix
} // namespace coco

#endif
