#ifndef COCO_COMBIX_PARSER_TRAIT_HPP_
#define COCO_COMBIX_PARSER_TRAIT_HPP_

#include <type_traits>

#include <coco/combix/error.hpp>
#include <coco/combix/parse_result.hpp>

namespace coco {
  namespace combix {

    struct is_parser_impl {
      template <typename P, typename S>
      static auto check(P* p) ->
          typename is_parse_result<decltype((*p)(std::declval<S&>()))>::type;

      template <typename P, typename S>
      static auto check(...) -> std::false_type;
    };

    template <typename P, typename S>
    struct is_parser : decltype(is_parser_impl::check<P, S>(nullptr)) {};

    template <typename P, typename S>
    constexpr bool is_parser_v = is_parser<P, S>::value;

    template <typename Parser, typename Stream>
    struct parse_result_of {
      static_assert(is_parser_v<Parser, Stream>,
                    "template argument is not a parser");
      using type = std::remove_reference_t<decltype(
          std::declval<Parser>()(std::declval<Stream&>()).unwrap())>;
    };

    template <typename Parser, typename Stream>
    using parse_result_of_t = typename parse_result_of<Parser, Stream>::type;

    template <typename Parser, typename Stream>
    struct parser_traits {
      using result_type = parse_result_of_t<Parser, Stream>;

      static parse_result<result_type, Stream> parse(Parser const& p,
                                                     Stream& s) {
        return p(s);
      }
    };

    template <typename P, typename S>
    auto parse(P const& parser, S& stream) {
      return parser_traits<P, S>::parse(parser, stream);
    }

  } // namespace combix
} // namespace coco

#endif
