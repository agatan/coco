#ifndef COCO_COMBIX_COMBINATORS_CHOICE_HPP_
#define COCO_COMBIX_COMBINATORS_CHOICE_HPP_

#include <type_traits>
#include <utility>

#include <coco/combix/error.hpp>
#include <coco/combix/parser_traits.hpp>

namespace coco {
  namespace combix {

    template <typename... P>
    struct choice_parser;

    template <typename P>
    struct choice_parser<P> {
      choice_parser(P const& p) : p(p) {}

      template <typename Stream>
      parse_result<parse_result_of_t<P, Stream>, Stream>
      operator()(Stream& s) const {
        return parse(p, s);
      }

      template <typename S>
      expected_list<typename stream_traits<S>::value_type> expected_info()
          const {
        return parser_traits<P, S>::expected_info(p);
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
          std::is_same<parse_result_of_t<Head, Stream>,
                       parse_result_of_t<base_type, Stream>>::value,
          parse_result<parse_result_of_t<Head, Stream>,
                       Stream>>::type
      operator()(Stream& s) const {
        auto res = parse(p, s);
        if (res) {
          return res;
        }
        res = base_type::operator()(s);
        if (res) {
          return res;
        }
        res.unwrap_error().set_expected(expected_info<Stream>());
        return res.unwrap_error();
      }

      template <typename S>
      expected_list<typename stream_traits<S>::value_type> expected_info()
          const {
        auto base = base_type::template expected_info<S>();
        auto info = parser_traits<Head, S>::expected_info(p);
        info.merge(base);
        return info;
      }

     private:
      Head p;
    };

    template <typename... Args>
    choice_parser<std::decay_t<Args>...> choice(Args&&... args) {
      return choice_parser<std::decay_t<Args>...>(std::forward<Args>(args)...);
    }

  } // namespace combix
} // namespace coco

#endif
