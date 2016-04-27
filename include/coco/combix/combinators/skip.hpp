#ifndef COCO_COMBIX_COMBINATORS_SKIP_HPP_
#define COCO_COMBIX_COMBINATORS_SKIP_HPP_

#include <coco/combix/parse_result.hpp>
#include <coco/combix/parser_traits.hpp>
#include <coco/combix/stream_traits.hpp>
#include <coco/combix/combinators/seq.hpp>
#include <coco/combix/combinators/map.hpp>

namespace coco {
  namespace combix {

    template <typename P, typename Skipper>
    struct skip_parser {
      explicit skip_parser(P&& p, Skipper&& s)
          : parser(std::move(p)), skipper(std::move(s)) {}
      explicit skip_parser(P const& p, Skipper const& s)
          : parser(p), skipper(s) {}

      template <typename Stream>
      parse_result<parse_result_of_t<P, Stream>, Stream> operator()(
          Stream& s) const {
        return parse(map(seq(parser, skipper), [](auto&& t) { return std::get<0>(t); }), s);
      }

      template <typename S>
      expected_list<typename stream_traits<S>::value_type> expected_info() const {
        return parser_traits<P, S>::expected_info(parser);
      }

     private:
      P parser;
      Skipper skipper;
    };

    template <typename P, typename Skipper>
    skip_parser<std::decay_t<P>, std::decay_t<Skipper>> skip(
        P&& parser, Skipper&& skipper) {
      return skip_parser<std::decay_t<P>, std::decay_t<Skipper>>(
          std::forward<P>(parser), std::forward<Skipper>(skipper));
    }

  } // namespace combix
} // namespace coco

#endif
