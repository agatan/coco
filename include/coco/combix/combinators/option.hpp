#ifndef COCO_COMBIX_COMBINATORS_OPTION_HPP_
#define COCO_COMBIX_COMBINATORS_OPTION_HPP_

#include <boost/optional.hpp>

#include <coco/combix/parse_result.hpp>
#include <coco/combix/stream_traits.hpp>
#include <coco/combix/parser_traits.hpp>

namespace coco {
  namespace combix {

    template <typename P>
    struct option_parser {
      explicit option_parser(P const& p) : parser(p) {}
      explicit option_parser(P&& p) : parser(std::move(p)) {}

      template <typename Stream>
      parse_result<boost::optional<parse_result_of_t<P, Stream>>, Stream>
      operator()(Stream& s) const {
        auto res = parse(parser, s);
        if (res) {
          return boost::optional<parse_result_of_t<P, Stream>>(res.unwrap());
        }
        return boost::optional<parse_result_of_t<P, Stream>>(boost::none);
      }

      template <typename Stream>
      expected_list<typename stream_traits<Stream>::value_type> expected_info()
          const {
        auto expected = coco::combix::expected_info<Stream>(parser);
        expected.nullable(true);
        return expected;
      }

    private:
      P parser;
    };

    template <typename P>
    option_parser<std::decay_t<P>> option(P&& parser) {
      return option_parser<std::decay_t<P>>(std::forward<P>(parser));
    }

  } // namespace combix
} // namespace coco

#endif
