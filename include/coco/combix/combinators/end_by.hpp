#ifndef COCO_COMBIX_COMBINATORS_END_by1_HPP_
#define COCO_COMBIX_COMBINATORS_END_by1_HPP_

#include <deque>
#include <type_traits>

#include <coco/combix/parse_result.hpp>
#include <coco/combix/stream_traits.hpp>
#include <coco/combix/parser_traits.hpp>
#include <coco/combix/combinators/map.hpp>
#include <coco/combix/combinators/seq.hpp>
#include <coco/combix/combinators/many.hpp>
#include <coco/combix/combinators/option.hpp>

namespace coco {
  namespace combix {

    template <typename P, typename End>
    struct end_by1_parser {
      end_by1_parser(P const& p, End const& s) : parser(p), end(s) {}
      end_by1_parser(P&& p, End&& s) : parser(std::move(p)), end(std::move(s)) {}

      template <typename Stream>
      parse_result<std::deque<parse_result_of_t<P, Stream>>, Stream>
      parse(Stream& s) const {
        auto p = map(seq(parser, end), [](auto&& v) {
          return std::get<0>(std::forward<decltype(v)>(v));
        });
        auto first = coco::combix::parse(p, s);
        if (!first) {
          return first.unwrap_error();
        }
        auto tail_parser = many(p);
        auto tail = coco::combix::parse(tail_parser, s);
        if (!tail) {
          if (tail.unwrap_error().consumed()) {
            return tail.unwrap_error();
          }
          return std::deque<parse_result_of_t<P, Stream>>{first.unwrap()};
        }
        (*tail).emplace_front(std::move(first.unwrap()));
        return *tail;
      }

      template <typename Stream>
      void add_error(parse_error<Stream>& err) const {
        parser.add_error(err);
      }

    private:
      P parser;
      End end;
    };

    template <typename P, typename End>
    end_by1_parser<std::decay_t<P>, std::decay_t<End>> end_by1(P&& p, End&& s) {
      return end_by1_parser<std::decay_t<P>, std::decay_t<End>>(
          std::forward<P>(p), std::forward<End>(s));
    }

    template <typename P, typename End>
    struct end_by_parser {
      end_by_parser(P const& p, End const& e)
          : parser(option(end_by1(p, e))) {}
      end_by_parser(P&& p, End&& e)
          : parser(option(end_by1(std::move(p), std::move(e)))) {}

      template <typename Stream>
      parse_result<std::deque<parse_result_of_t<P, Stream>>, Stream>
      parse(Stream& s) const {
        auto const p = map(parser, [](auto&& opt) {
          if (opt) {
            return *opt;
          }
          return std::deque<parse_result_of_t<P, Stream>>{};
        });
        return coco::combix::parse(p, s);
      }

      template <typename Stream>
      void add_error(parse_error<Stream>& err) const {
        parser.add_error(err);
      }

    private:
      option_parser<end_by1_parser<P, End>> parser;
    };

    template <typename P, typename End>
    end_by_parser<std::decay_t<P>, std::decay_t<End>> end_by(P&& p, End&& s) {
      return end_by_parser<std::decay_t<P>, std::decay_t<End>>(
          std::forward<P>(p), std::forward<End>(s));
    }

  } // namespace combix
} // namespace coco

#endif
