#ifndef COCO_COMBIX_COMBINATORS_SEP_BY_HPP_
#define COCO_COMBIX_COMBINATORS_SEP_BY_HPP_

#include <list>
#include <type_traits>

#include <coco/combix/parse_result.hpp>
#include <coco/combix/stream_traits.hpp>
#include <coco/combix/parser_traits.hpp>
#include <coco/combix/combinators/map.hpp>
#include <coco/combix/combinators/seq.hpp>
#include <coco/combix/combinators/many.hpp>

namespace coco {
  namespace combix {

    template <typename P, typename Sep>
    struct sep_by_parser {
      sep_by_parser(P const& p, Sep const& s) : parser(p), sep(s) {}
      sep_by_parser(P&& p, Sep&& s) : parser(std::move(p)), sep(std::move(s)) {}

      template <typename Stream>
      parse_result<std::list<parse_result_of_t<P, Stream>>, Stream>
      operator()(Stream& s) const {
        auto first = parse(parser, s);
        if (!first) {
          return std::list<parse_result_of_t<P, Stream>>{};
        }
        auto tail_parser = many(map(seq(sep, parser), [](auto&& v) {
          return std::get<1>(std::forward<decltype(v)>(v));
        }));
        auto tail = parse(tail_parser, s);
        if (!tail) {
          return std::list<parse_result_of_t<P, Stream>>{first.unwrap()};
        }
        (*tail).emplace_front(std::move(first.unwrap()));
        return *tail;
      }

      template <typename S>
      expected_list<typename stream_traits<S>::value_type> expected_info() const {
        auto head = parser_traits<P, S>::expected_info(parser);
        if (head.nullable()) {
          head.merge(parser_traits<Sep, S>::expected_info(sep));
        }
        return head;
      }

    private:
      P parser;
      Sep sep;
    };

    template <typename P, typename Sep>
    sep_by_parser<std::decay_t<P>, std::decay_t<Sep>> sep_by(P&& p, Sep&& s) {
      return sep_by_parser<std::decay_t<P>, std::decay_t<Sep>>(
          std::forward<P>(p), std::forward<Sep>(s));
    }

  } // namespace combix
} // namespace coco

#endif
