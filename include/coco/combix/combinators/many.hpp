#ifndef COCO_COMBIX_COMBINATORS_MANY_HPP_
#define COCO_COMBIX_COMBINATORS_MANY_HPP_

#include <deque>
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
      parse_result<std::deque<parse_result_of_t<P, Stream>>, Stream>
      parse(Stream& s) const {
        std::deque<parse_result_of_t<P, Stream>> result;
        for (auto r = parser.parse(s); r; r = parser.parse(s)) {
          result.push_back(*r);
        }
        return result;
      }

      template <typename Stream>
      void add_error(parse_error<Stream>&) const { }
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
      parse_result<std::deque<parse_result_of_t<P, Stream>>, Stream>
      parse(Stream& s) const {
        auto res = parser.parse(s);
        if (!res) {
          return {res.unwrap_error()};
        }
        auto tail = many(parser).parse(s);
        if (!tail) {
          return tail;
        }
        tail.unwrap().emplace_front(std::move(res.unwrap()));
        return tail;
      }

      template <typename Stream>
      void add_error(parse_error<Stream>& err) const {
        parser.add_error(err);
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
