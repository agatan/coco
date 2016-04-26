#ifndef COCO_COMBIX_COMBINATORS_CHAINL_HPP_
#define COCO_COMBIX_COMBINATORS_CHAINL_HPP_

#include <type_traits>

#include <coco/combix/parser_traits.hpp>
#include <coco/combix/parse_result.hpp>

namespace coco {
  namespace combix {

    template <typename P, typename Op>
    struct chainl1_parser {
      chainl1_parser(P const& p, Op const& op) : parser(p), op_parser(op) {}


      template <typename Stream>
      parse_result<parse_result_of_t<P, Stream>, Stream> operator()(
          Stream& s) const {
        using result_type = parse_result_of_t<P, Stream>;
        using op_result_type = parse_result_of_t<Op, Stream>;
        static_assert(
            std::is_same<result_type, decltype(std::declval<op_result_type>()(
                                          std::declval<result_type&&>(),
                                          std::declval<result_type&&>()))>::value,
            "chain operator's return type is wrong");

        auto res = parse(parser, s);
        if (!res) {
          res.unwrap_error().set_expected(expected_info<Stream>());
          return res.unwrap_error();
        }
        auto lhs = *res;
        auto op = parse(op_parser, s);
        while (op.is_ok()) {
          auto rhs = parse(parser, s);
          if (!rhs) {
            rhs.unwrap_error().set_expected(expected_info<Stream>());
            return rhs;
          }
          auto tmp = op.unwrap()(std::move(lhs), std::move(*rhs));
          lhs = std::move(tmp);
          op = parse(op_parser, s);
        }
        return {lhs};
      }

      template <typename S>
      expected_list<typename stream_traits<S>::value_type> expected_info()
          const {
        auto current = parser_traits<P, S>::expected_info(parser);
        if (current.nullable()) {
          current.merge(parser_traits<Op, S>::expected_info(op_parser));
        }
        return current;
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
