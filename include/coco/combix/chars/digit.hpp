#ifndef COCO_COMBIX_CHARS_DIGIT_HPP_
#define COCO_COMBIX_CHARS_DIGIT_HPP_

#include <coco/combix/primitives.hpp>
#include <coco/combix/parse_result.hpp>
#include <coco/combix/parser_traits.hpp>

namespace coco {
  namespace combix {

    struct digit_parser {
      template <typename Stream>
      parse_result<int, Stream> operator()(Stream& s) const {
        auto res = parse(satisfy([](auto c) { return '0' <= c && c <= '9'; }), s);
        if (!res) {
          return res.unwrap_error();
        }
        return static_cast<int>(res.unwrap()) - '0';
      }

      template <typename S>
      expected_list<typename stream_traits<S>::value_type> expected_info()
          const {
        return expected_list<typename stream_traits<S>::value_type>(
            std::string{"digit"});
      }
    };

    digit_parser digit() {
      return {};
    }

  } // namespace combix
} // namespace coco

#endif
