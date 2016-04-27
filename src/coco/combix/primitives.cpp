#include <coco/combix/primitives.hpp>

namespace coco {
  namespace combix {
    any_parser any() { return any_parser{}; }

    eof_parser eof() { return eof_parser{}; }
  } // namespace combix
} // namespace coco
