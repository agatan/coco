#include <coco/combix/chars/alpha.hpp>

#include <cctype>

namespace coco {
  namespace combix {

    bool isalpha(char c) {
      return static_cast<bool>(std::isalpha(c));
    }

    alpha_parser alpha() {
      return expected(satisfy(isalpha), "alphabet");
    }

  } // namespace combix
} // namespace coco
