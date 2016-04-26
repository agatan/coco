#ifndef COCO_COMBIX_COMBINATORS_BETWEEN_HPP_
#define COCO_COMBIX_COMBINATORS_BETWEEN_HPP_

#include <coco/combix/parse_result.hpp>
#include <coco/combix/parser_traits.hpp>
#include <coco/combix/combinators/map.hpp>
#include <coco/combix/combinators/seq.hpp>

namespace coco {
  namespace combix {

    template <typename Open, typename Close, typename Parser>
    struct between_parser {
      template <typename O, typename C, typename P>
      between_parser(O&& o, C&& c, P&& p)
          : open(std::forward<O>(o)),
            close(std::forward<C>(c)),
            parser(std::forward<P>(p)) {}

      template <typename Stream>
      parse_result<parse_result_of_t<Parser, Stream>, Stream> operator()(
          Stream& s) const {
        auto op = parse(open, s);
        if (!op) {
          return {op.unwrap_error()};
        }
        auto res = parse(parser, s);
        if (!res) {
          return res;
        }
        auto cl = parse(close, s);
        if (!cl) {
          return {cl.unwrap_error()};
        }
        return res;
      }

      template <typename S>
      expected_list<typename stream_traits<S>::value_type> expected_info()
          const {
        auto o = parser_traits<Open, S>::expected_info(open);
        if (o.nullable()) {
          o.merge(parser_traits<Parser, S>::expected_info(parser));
        }
        if (o.nullable()) {
          o.merge(parser_traits<Close, S>::expected_info(close));
        }
        return o;
      }

     private:
      Open open;
      Close close;
      Parser parser;
    };

    template <typename Open, typename Close, typename Parser>
    between_parser<std::decay_t<Open>, std::decay_t<Close>,
                   std::decay_t<Parser>>
    between(Open&& o, Close&& c, Parser&& p) {
      return {std::forward<Open>(o), std::forward<Close>(c),
              std::forward<Parser>(p)};
    }

  } // namespace combix
} // namespace coco

#endif
