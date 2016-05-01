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
      parse(Stream& s) const {
        auto res = parser.parse(s);
        if (res) {
          return boost::optional<parse_result_of_t<P, Stream>>(res.unwrap());
        } else if (res.unwrap_error().consumed()) {
          return res.unwrap_error();
        }
        return boost::optional<parse_result_of_t<P, Stream>>(boost::none);
      }

      template <typename Stream>
      void add_error(parse_error<Stream>& err) const {
        parser.add_error(err);
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
