#ifndef COCO_COMBIX_COMBINATORS_MAP_HPP_
#define COCO_COMBIX_COMBINATORS_MAP_HPP_

#include <type_traits>

#include <coco/combix/error.hpp>
#include <coco/combix/stream_trait.hpp>
#include <coco/combix/parser_trait.hpp>

namespace coco {
  namespace combix {

    template <typename P, typename F>
    struct map_parser {
      template <typename Stream>
      using result_type = typename std::result_of<F(
          typename parser_trait<P>::template result_type<Stream>)>::type;

      map_parser(P const& p, F const& f) : p(p), f(f) {}

      template <typename Stream>
      parse_result<result_type<Stream>, Stream> parse(Stream& s) const {
        return parser_trait<P>::parse(p, s).map(f);
      }

    private:
      P p;
      F f;
    };

    template <typename P, typename F>
    struct is_parser<map_parser<P, F>> : std::true_type {};

    template <typename P, typename F>
    map_parser<P, F> map(P&& p, F&& f) {
      return {p, f};
    }
    
  } // namespace combix
} // namespace coco

#endif
