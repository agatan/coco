#ifndef COCO_COMBIX_COMBINATORS_CHOICE_HPP_
#define COCO_COMBIX_COMBINATORS_CHOICE_HPP_

#include <type_traits>
#include <utility>

#include <coco/combix/parser_trait.hpp>

namespace coco {
  namespace combix {

    template <typename... P>
    struct choice_parser;

    template <typename P>
    struct choice_parser<P> {
      template <typename Stream>
      using result_type =
          typename parser_trait<P>::template result_type<Stream>;

      choice_parser(P const& p) : p(p) {}

      template <typename Stream>
      parse_result<result_type<Stream>, Stream> parse(Stream& s) const {
        return parser_trait<P>::parse(p, s);
      }
    private:
      P p;
    };

    template <typename Head, typename... Tail>
    struct choice_parser<Head, Tail...> : choice_parser<Tail...> {
      using base_type = choice_parser<Tail...>;

      template <typename Stream>
      using result_type = typename std::enable_if<
          std::is_same<
              typename parser_trait<Head>::template result_type<Stream>,
              typename parser_trait<base_type>::template result_type<Stream>>::
              value,
          typename parser_trait<Head>::template result_type<Stream>>::type;

      choice_parser(Head const& h, Tail const&... tail)
        : choice_parser<Tail...>(tail...), p(h) {}

      template <typename Stream>
      parse_result<result_type<Stream>, Stream> parse(Stream& s) const {
        auto res = parser_trait<Head>::parse(p, s);
        if (res) {
          return res;
        }
        return choice_parser<Tail...>::parse(s);
      }

    private:
      Head p;
    };

    template <typename... Args>
    struct is_parser<choice_parser<Args...>> : std::true_type {};

    template <typename... Args>
    choice_parser<Args...> choice(Args&&... args) {
      return choice_parser<Args...>(std::forward<Args>(args)...);
    }

  } // namespace combix
} // namespace coco

#endif
