#ifndef COCO_COMBIX_COMBINATORS_CHOICE_HPP_
#define COCO_COMBIX_COMBINATORS_CHOICE_HPP_

#include <type_traits>
#include <utility>

#include <coco/combix/parser_traits.hpp>

template <typename>
struct TD;

namespace coco {
  namespace combix {

    template <typename... P>
    struct choice_parser;

    template <typename P>
    struct choice_parser<P> {
      choice_parser(P const& p) : p(p) {}

      template <typename Stream>
      parse_result<parse_result_of_t<std::decay_t<P>, Stream>, Stream>
      operator()(Stream& s) const {
        static_assert(is_parser_v<std::decay_t<P>, Stream>,
                      "template argument is not a parser");
        return parse(p, s);
      }

     private:
      P p;
    };

    template <typename Head, typename... Tail>
    struct choice_parser<Head, Tail...> : choice_parser<Tail...> {
      using base_type = choice_parser<Tail...>;

      choice_parser(Head const& h, Tail const&... tail)
        : choice_parser<Tail...>(tail...), p(h) {}

      template <typename Stream>
      typename std::enable_if<
          std::is_same<parse_result_of_t<std::decay_t<Head>, Stream>,
                       parse_result_of_t<base_type, Stream>>::value,
          parse_result<parse_result_of_t<std::decay_t<Head>, Stream>,
                       Stream>>::type
      operator()(Stream& s) const {
        static_assert(is_parser_v<std::decay_t<Head>, Stream>,
                      "template argument is not a parser");
        auto res = parse(p, s);
        if (res) {
          return res;
        }
        return base_type::operator()(s);
      }

     private:
      Head p;
    };

    template <typename... Args>
    choice_parser<Args...> choice(Args&&... args) {
      return choice_parser<Args...>(std::forward<Args>(args)...);
    }

  } // namespace combix
} // namespace coco

#endif
