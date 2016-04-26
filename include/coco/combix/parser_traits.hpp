#ifndef COCO_COMBIX_PARSER_TRAIT_HPP_
#define COCO_COMBIX_PARSER_TRAIT_HPP_

#include <type_traits>
#include <list>

#include <coco/combix/error.hpp>
#include <coco/combix/parse_result.hpp>

namespace coco {
  namespace combix {

    struct has_expected_info_impl {
      template <typename P, typename S>
      static auto check(P* p) -> decltype(
          p->template expected_info<S>(),
          std::true_type{});

      template <typename, typename>
      static auto check(...) -> std::false_type;
    };

    template <typename P, typename S>
    struct has_expected_info
        : public decltype(has_expected_info_impl::check<P, S>(nullptr)) {};

    template <typename P, typename S>
    constexpr bool has_expected_info_v = has_expected_info<P, S>::value;

    struct is_parser_impl {
      template <typename P, typename S>
      static auto check(P* p) ->
          typename is_parse_result<decltype((*p)(std::declval<S&>()))>::type;

      template <typename P, typename S>
      static auto check(...) -> std::false_type;
    };

    template <typename P, typename S>
    struct is_parser {
      static constexpr bool value =
          decltype(is_parser_impl::check<P, S>(nullptr))::value &&
          has_expected_info_v<P, S>;
    };

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

      static expected_list<typename stream_traits<Stream>::value_type>
      expected_info(Parser const& parser) {
        return parser.template expected_info<Stream>();
      }
    };

    template <typename P, typename S>
    auto parse(P const& parser, S& stream) {
      return parser_traits<P, S>::parse(parser, stream);
    }

    template <typename S, typename P>
    expected_list<typename stream_traits<S>::value_type>
    expected_info(P const& p) {
      return parser_traits<P, S>::expected_info(p);
    }

  } // namespace combix
} // namespace coco

#endif
