#include <coco/combix/chars/space.hpp>

namespace coco {
  namespace combix {

    space_parser space() {
      return {};
    }

    expected_parser<many_parser<space_parser>> spaces() {
      return expected(many(space()), "whitespaces");
    }

  } // namespace combix
} // namespace coco
