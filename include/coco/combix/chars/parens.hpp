#ifndef COCO_COMBIX_CHARS_PARENS_HPP_
#define COCO_COMBIX_CHARS_PARENS_HPP_

#include <coco/combix/parser_traits.hpp>
#include <coco/combix/parse_result.hpp>
#include <coco/combix/combinators/between.hpp>

namespace coco {
  namespace combix {

    template <typename P>
    struct parens_parser {
      explicit parens_parser(P const& p) : parser(p) {}
      explicit parens_parser(P&& p) : parser(std::move(p)) {}

      template <typename Stream>
      parse_result<parse_result_of_t<P, Stream>, Stream>
      operator()(Stream& s) const {
        return parse(between(token('('), token(')'), parser), s);
      }

    private:
      P parser;
    };

    template <typename P>
    auto parens(P&& parser) {
      return between(token('('), token(')'), parser);
    }

  } // namespace combix
} // namespace coco

#endif

