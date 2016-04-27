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

    struct space_parser {
      template <typename Stream>
      parse_result<unused, Stream> operator()(Stream& s) const {
        auto res = parse(expected(satisfy([](auto c) { return std::isspace(c); }),
                              "whitespace"),
                     s);
        if (res) {
          return unused();
        } else {
          return res.unwrap_error();
        }
      }

      template <typename Stream>
      expected_list<typename stream_traits<Stream>::value_type> expected_info()
          const {
        return expected_list<typename stream_traits<Stream>::value_type>(
            "whitespace");
      }
    };

    space_parser space();

    expected_parser<many_parser<space_parser>> spaces();

  } // namespace combix
} // namespace coco

#endif
