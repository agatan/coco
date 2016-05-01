#include <coco/combix/chars/space.hpp>

#include <cctype>

namespace coco {
  namespace combix {

    bool isspace(char c) {
      return static_cast<bool>(std::isspace(c));
    }

    space_parser space() {
      return expected(satisfy(isspace), "space");
    }

    expected_parser<many_parser<space_parser>> spaces() {
      return expected(many(space()), "whitespaces");
    }

  } // namespace combix
} // namespace coco
