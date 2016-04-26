#ifndef COCO_COMBIX_COMBINATORS_MAP_HPP_
#define COCO_COMBIX_COMBINATORS_MAP_HPP_

#include <type_traits>

#include <coco/combix/error.hpp>
#include <coco/combix/parser_traits.hpp>

namespace coco {
  namespace combix {

    template <typename P, typename F>
    struct map_parser {
      template <typename Stream>
      using result_type = typename std::result_of<F(
          parse_result_of_t<P, Stream>)>::type;

      map_parser(P const& p, F const& f) : p(p), f(f) {}

      template <typename Stream>
      parse_result<result_type<Stream>, Stream> operator()(Stream& s) const {
        return parse(p, s).map(f);
      }

      template <typename S>
      expected_list<typename stream_traits<S>::value_type> expected_info()
          const {
        return parser_traits<P, S>::expected_info(p);
      }

    private:
      P p;
      F f;
    };

    template <typename P, typename F>
    map_parser<std::decay_t<P>, std::decay_t<F>> map(P&& p, F&& f) {
      return {p, f};
    }
    
  } // namespace combix
} // namespace coco

#endif
