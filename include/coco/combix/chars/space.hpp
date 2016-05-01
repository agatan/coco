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

    space_parser space();

    expected_parser<many_parser<space_parser>> spaces();

  } // namespace combix
} // namespace coco

#endif
