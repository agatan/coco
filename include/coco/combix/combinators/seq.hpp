#ifndef COCO_COMBIX_COMBINATORS_SEQ_HPP_
#define COCO_COMBIX_COMBINATORS_SEQ_HPP_

#include <tuple>
#include <type_traits>

#include <coco/combix/error.hpp>

namespace coco {
  namespace combix {

    namespace detail {
      template <typename...>
      struct tuple_add;

      template <typename Arg, typename... Data>
      struct tuple_add<Arg, std::tuple<Data...>> {
        using type = std::tuple<std::decay_t<Arg>, Data...>;
      };

      template <typename Arg, typename Tuple>
      using tuple_add_t = typename tuple_add<Arg, Tuple>::type;

      template <typename Arg, typename Tuple>
      tuple_add_t<std::decay_t<Arg>, std::decay_t<Tuple>> add_tuple(Arg&& a,
                                                                    Tuple&& t) {
        return std::tuple_cat(std::make_tuple(std::forward<Arg>(a)),
                              std::forward<Tuple>(t));
      }
    } // namespace detail

    template <typename...>
    struct seq_parser;

    template <typename P>
    struct seq_parser<P> {
      template <typename Stream>
      using result_type =
          std::tuple<parse_result_of_t<std::decay_t<P>, Stream>>;

      seq_parser(P const& p) : p(p) {}

      template <typename Stream>
      parse_result<result_type<Stream>, Stream> operator()(Stream& s) const {
        static_assert(is_parser_v<std::decay_t<P>, Stream>,
                      "template argument is not a parser");
        return parse(p, s).map([](auto&& r) { return std::make_tuple(r); });
      }

     private:
      P p;
    };

    template <typename Head, typename... Tail>
    struct seq_parser<Head, Tail...> : seq_parser<Tail...> {
      using base_type = seq_parser<Tail...>;

      template <typename Stream>
      using result_type =
          detail::tuple_add_t<parse_result_of_t<std::decay_t<Head>, Stream>,
                              parse_result_of_t<base_type, Stream>>;

      seq_parser(Head const& h, Tail const&... tail)
          : seq_parser<Tail...>(tail...), p(h) {}

      template <typename Stream>
      parse_result<result_type<Stream>, Stream> operator()(Stream& s) const {
        auto res = parse(p, s);
        if (!res) {
          return res.unwrap_error();
        }
        auto tail = base_type::operator()(s);
        if (!tail) {
          return tail.unwrap_error();
        }
        return detail::add_tuple(*res, *tail);
      }

    private:
        Head p;
    };

    template <typename... Args>
    seq_parser<Args...> seq(Args&&... args) {
      return {std::forward<Args>(args)...};
    }

  } // namespace combix
} // namespace coco

#endif
