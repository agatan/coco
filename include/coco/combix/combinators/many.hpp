#ifndef COCO_COMBIX_COMBINATORS_MANY_HPP_
#define COCO_COMBIX_COMBINATORS_MANY_HPP_

#include <vector>
#include <type_traits>

#include <coco/combix/parse_result.hpp>
#include <coco/combix/parser_traits.hpp>

namespace coco {
  namespace combix {

    template <typename P>
    struct many_parser {
      many_parser(P const& p) : parser(p) {}
      many_parser(P&& p) : parser(std::move(p)) {}

      ~many_parser() = default;

      template <typename Stream>
      parse_result<std::vector<parse_result_of_t<P, Stream>>, Stream>
      operator()(Stream& s) const {
        static_assert(is_parser_v<P, Stream>,
                      "template argument is not a parser");
        std::vector<parse_result_of_t<P, Stream>> result;
        for (auto r = parser(s); r; r = parser(s)) {
          result.push_back(*r);
        }
        return result;
      }
    private:
      P parser;
    };

    template <typename P>
    many_parser<std::decay_t<P>> many(P&& p) {
      return many_parser<std::decay_t<P>>(std::forward<P>(p));
    }

  } // namespace combix
} // namespace coco

#endif
