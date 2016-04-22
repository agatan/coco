#ifndef COCO_COMBIX_PARSER_TRAIT_HPP_
#define COCO_COMBIX_PARSER_TRAIT_HPP_

#include <type_traits>

#include <coco/combix/error.hpp>
#include <coco/combix/parse_result.hpp>

namespace coco {
  namespace combix {

    template <typename P>
    struct parser_trait {
      template <typename Stream>
      using result_type = typename std::decay_t<P>::template result_type<Stream>;

      template <typename Stream>
      static parse_result<result_type<Stream>, Stream> parse(P const& p,
                                                             Stream& s) {
        return p.parse(s);
      }
    };

    template <typename T>
    struct is_parser : std::false_type {};

    template <typename T>
    constexpr bool is_parser_v = is_parser<T>::value;

  } // namespace combix
} // namespace coco

#endif
