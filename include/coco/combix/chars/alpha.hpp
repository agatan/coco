#ifndef COCO_COMBIX_CHARS_ALPHA_HPP_
#define COCO_COMBIX_CHARS_ALPHA_HPP_

#include <coco/combix/primitives.hpp>
#include <coco/combix/parse_result.hpp>
#include <coco/combix/parser_traits.hpp>

namespace coco {
  namespace combix {

    struct alpha_parser {
      template <typename Stream>
      parse_result<char, Stream> operator()(Stream& s) const {
        return parse(satisfy([](auto c) { return 'A' <= c && c <= 'z'; }), s);
      }
    };

    alpha_parser alpha() {
      return {};
    }

  } // namespace combix
} // namespace coco

#endif
