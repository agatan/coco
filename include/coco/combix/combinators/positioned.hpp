#ifndef COCO_COMBIX_COMBINATORS_POSITIONED_HPP_
#define COCO_COMBIX_COMBINATORS_POSITIONED_HPP_

#include <type_traits>

#include <coco/combix/parse_result.hpp>
#include <coco/combix/primitives.hpp>
#include <coco/combix/combinators/seq.hpp>
#include <coco/combix/combinators/map.hpp>

namespace coco {
  namespace combix {

    template <typename P>
    struct positioned_parser {
      explicit positioned_parser(P const& p) : parser(p) {}
      explicit positioned_parser(P&& p) : parser(std::move(p)) {}

      template <typename Stream,
                std::enable_if_t<detail::has_position_v<Stream>,
                                 std::nullptr_t> = nullptr>
      parse_result<std::pair<typename Stream::position_type,
                             parse_result_of_t<P, Stream>>,
                   Stream>
      parse(Stream& s) const {
        return coco::combix::parse(map(seq(getpos(), parser),
                                       [](auto&& t) {
                                         return std::make_pair(
                                             std::move(std::get<0>(t)),
                                             std::move(std::get<1>(t)));
                                       }),
                                   s);
      }

      template <typename Stream,
                std::enable_if_t<detail::has_position_v<Stream>,
                                 std::nullptr_t> = nullptr>
      void add_error(parse_error<Stream>& err) const {
        parser.add_error(err);
      }

    private:
      P parser;
    };

    template <typename Parser>
    positioned_parser<std::decay_t<Parser>> positioned(Parser&& p) {
      return positioned_parser<std::decay_t<Parser>>(std::forward<Parser>(p));
    }

  } // namespace combix
} // namespace coco

#endif
