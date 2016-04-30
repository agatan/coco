#ifndef COCO_COMBIX_COMBINATORS_TRY__HPP_
#define COCO_COMBIX_COMBINATORS_TRY__HPP_

#include <type_traits>

#include <coco/combix/parse_result.hpp>
#include <coco/combix/stream_traits.hpp>
#include <coco/combix/parser_traits.hpp>

namespace coco {
  namespace combix {

    template <typename P>
    struct try_parser {
      explicit try_parser(P const& p) : parser(p) {}
      explicit try_parser(P&& p) : parser(std::move(p)) {}

      template <typename Stream>
      parse_result<parse_result_of_t<P, Stream>, Stream>
      operator()(Stream& s) const {
        auto saved = save(s);
        auto res = parse(parser, s);
        if (!res) {
          restore(s, std::move(saved));
          return res.unwrap_error();
        }
        return res.unwrap();
      }

      template <typename Stream>
      expected_list<typename stream_traits<Stream>::value_type> expected_info()
          const {
        return expected_info<Stream>(parser);
      }

    private:
      P parser;
    };;

    template <typename P>
    try_parser<std::decay_t<P>> try_(P&& p) {
      return try_parser<std::decay_t<P>>(std::forward<P>(p));
    }

  } // namespace combix
} // namespace coco

#endif
