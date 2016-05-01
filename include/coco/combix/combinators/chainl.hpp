#ifndef COCO_COMBIX_COMBINATORS_CHAINL_HPP_
#define COCO_COMBIX_COMBINATORS_CHAINL_HPP_

#include <type_traits>

#include <coco/combix/parser_traits.hpp>
#include <coco/combix/parse_result.hpp>
#include <coco/combix/combinators/seq.hpp>

namespace coco {
  namespace combix {

    template <typename P, typename Op>
    struct chainl1_parser {
      chainl1_parser(P const& p, Op const& op) : parser(p), op_parser(op) {}


      template <typename Stream>
      parse_result<parse_result_of_t<P, Stream>, Stream> parse(
          Stream& s) const {
        using result_type = parse_result_of_t<P, Stream>;
        using op_result_type = parse_result_of_t<Op, Stream>;
        static_assert(
            std::is_same<result_type, decltype(std::declval<op_result_type>()(
                                          std::declval<result_type&&>(),
                                          std::declval<result_type&&>()))>::value,
            "chain operator's return type is wrong");

        auto res = coco::combix::parse(parser, s);
        if (!res) {
          return res.unwrap_error();
        }
        auto lhs = *res;
        for (;;) {
          auto op_rhs = coco::combix::parse(seq(op_parser, parser), s);
          if (op_rhs) {
            auto&& op = std::get<0>(*op_rhs);
            auto&& rhs = std::get<1>(*op_rhs);
            lhs = op(std::move(lhs), std::move(rhs));
          } else if (op_rhs.unwrap_error().consumed()) {
            return op_rhs.unwrap_error();
          } else {
            return lhs;
          }
        }
      }

      template <typename Stream>
      void add_error(parse_error<Stream>& err) const {
        parser.add_error(err);
      }

    private:
      P parser;
      Op op_parser;
    };

    template <typename P, typename Op>
    chainl1_parser<std::decay_t<P>, std::decay_t<Op>> chainl1(P&& p, Op&& op) {
      return {std::forward<P>(p), std::forward<Op>(op)};
    }

  } // namespace combix
} // namespace coco

#endif
