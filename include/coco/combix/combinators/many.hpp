#ifndef COCO_COMBIX_COMBINATORS_MANY_HPP_
#define COCO_COMBIX_COMBINATORS_MANY_HPP_

#include <list>
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
      parse_result<std::list<parse_result_of_t<P, Stream>>, Stream>
      operator()(Stream& s) const {
        std::list<parse_result_of_t<P, Stream>> result;
        for (auto r = parse(parser, s); r; r = parse(parser, s)) {
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

    template <typename P>
    struct many1_parser {
      many1_parser(P const& p) : parser(p) {}
      many1_parser(P&& p) : parser(std::move(p)) {}

      ~many1_parser() = default;

      template <typename Stream>
      parse_result<std::list<parse_result_of_t<P, Stream>>, Stream>
      operator()(Stream& s) const {
        auto res = parse(parser, s);
        if (!res) {
          return {res.unwrap_error()};
        }
        auto tail = parse(many(parser), s);
        if (!tail) {
          return tail;
        }
        tail.unwrap().push_front(res.unwrap());
        return tail;
      }

    private:
      P parser;
    };

    template <typename P>
    many1_parser<std::decay_t<P>> many1(P&& p) {
      return {std::forward<P>(p)};
    }


  } // namespace combix
} // namespace coco

#endif
