#include <coco/combix/chars/string.hpp>

namespace coco {
  namespace combix {

    string_parser string(std::string const& s) {
      return string_parser(s);
    }

    string_parser string(std::string&& s) {
      return string_parser(std::move(s));
    }

  } // namespace combix
} // namespace coco
