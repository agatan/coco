#include <coco/combix/chars/digit.hpp>

namespace coco {
  namespace combix {

    bool isdigit(char c) {
      return static_cast<bool>(std::isdigit(c));
    }

    digit_parser digit() {
      return expected(satisfy(isdigit), "digit");
    }
  } // namespace combix
} // namespace coco
