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
      parse(Stream& s) const {
        return coco::combix::parse(p, s);
      }

      template <typename Stream>
      void add_error(parse_error<Stream>& err) const {
        p.add_error(err);
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
      parse(Stream& s) const {
        auto res = coco::combix::parse(p, s);
        if (res) {
          return res;
        } else if (res.unwrap_error().consumed()) {
          return res.unwrap_error();
        }
        auto tail_res = base_type::parse(s);
        if (tail_res) {
          return tail_res;
        }
        res.unwrap_error().merge(std::move(tail_res.unwrap_error()));
        return res.unwrap_error();
      }

      template <typename Stream>
      void add_error(parse_error<Stream>& err) const {
        p.add_error(err);
        base_type::add_error(err);
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
