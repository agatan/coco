#ifndef COCO_COMBIX_CHARS_SPACE_HPP_
#define COCO_COMBIX_CHARS_SPACE_HPP_

#include <cctype>

#include <coco/combix/unused.hpp>
#include <coco/combix/parse_result.hpp>
#include <coco/combix/primitives.hpp>
#include <coco/combix/combinators/expected.hpp>
#include <coco/combix/combinators/many.hpp>

namespace coco {
  namespace combix {

    using space_parser = expected_parser<satisfy_parser<bool(*)(char)>>;

    inline bool isspace(char c) {
      return static_cast<bool>(std::isspace(c));
    }

    inline space_parser space() {
      return expected(satisfy(isspace), "space");
    }

    inline expected_parser<many_parser<space_parser>> spaces() {
      return expected(many(space()), "whitespaces");
    }

  } // namespace combix
} // namespace coco

#endif
