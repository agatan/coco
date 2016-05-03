#ifndef COCO_COMBIX_COMBINATORS_NOT_FOLLOWED_BY_HPP_
#define COCO_COMBIX_COMBINATORS_NOT_FOLLOWED_BY_HPP_

#include <sstream>

#include <coco/combix/stream_traits.hpp>
#include <coco/combix/parser_traits.hpp>
#include <coco/combix/combinators/try_.hpp>
#include <coco/combix/unused.hpp>

namespace coco {
  namespace combix {

    template <typename P>
    struct not_followed_by_parser {
      not_followed_by_parser(P const& p) : parser(p) {}
      not_followed_by_parser(P&& p) : parser(std::move(p)) {}

      template <typename Stream>
      parse_result<unused, Stream>
      parse(Stream& s) const {
        auto saved = save(s);
        auto res = coco::combix::parse(parser, s);
        restore(s, std::move(saved));
        if (res) {
          std::stringstream ss;
          ss << *res;
          return parse_error<Stream>(s, make_unexpected<Stream>(ss.str()));
        }
        return unused{};
      }

      template <typename Stream>
      void add_error(parse_error<Stream>&) const {
        // TODO: unexpected combinator
      }

     private:
      P parser;
    };

    template <typename P>
    not_followed_by_parser<std::decay_t<P>>
    not_followed_by(P&& p) {
      return not_followed_by_parser<std::decay_t<P>>(std::forward<P>(p));
    }

  } // namespace combix
} // namespace coco

#endif
