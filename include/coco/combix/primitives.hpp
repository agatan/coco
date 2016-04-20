#ifndef COCO_COMBIX_PRIMITIVES_HPP_
#define COCO_COMBIX_PRIMITIVES_HPP_

#include <coco/combix/error.hpp>
#include <coco/combix/parse_result.hpp>
#include <coco/combix/parser_trait.hpp>
#include <coco/combix/stream_trait.hpp>
#include <coco/expected.hpp>

namespace coco {
  namespace combix {

    struct any_parser {
      template <typename Stream>
      using result_type = typename stream_trait<Stream>::value_type;

      template <typename Stream>
      parse_result<result_type<Stream>, Stream> parse(Stream& s) const {
        auto res = uncons(s);
        if (res) {
          return *res;
        }
        return {res.unwrap_error()};
      }
    };

    template <>
    struct is_parser<any_parser> : std::true_type {};

    any_parser any() {
      return any_parser{};
    }

    template <typename F>
    struct satisfy_parser {
      template <typename Stream>
      using result_type = typename stream_trait<Stream>::value_type;

      satisfy_parser(F&& f) : f(std::move(f)) {}
      satisfy_parser(F const& f) : f(f) {}

      template <typename Stream>
      parse_result<result_type<Stream>, Stream> parse(Stream& s) const {
        auto res = peek(s);
        if (!res) {
          return {res.unwrap_error()};
        }
        auto token = res.unwrap();
        if (f(token)) {
          uncons(s);
          return token;
        }
        return {error<typename stream_trait<Stream>::value_type>(
            error_type::unexpected, token)};
      }

    private:
      F f;
    };

    template <typename F>
    struct is_parser<satisfy_parser<F>> : std::true_type {};

    template <typename F>
    satisfy_parser<F> satisfy(F&& f) {
      return satisfy_parser<F>(std::forward<F>(f));
    }

    template <typename V>
    struct token_parser {
      template <typename Stream>
      using result_type = typename stream_trait<Stream>::value_type;

      token_parser(V v) : v(std::move(v)) {}

      template <typename Stream>
      parse_result<result_type<Stream>, Stream> parse(Stream& s) const {
        auto res = peek(s);
        if (!res) {
          return {res.unwrap_error()};
        }
        auto t = res.unwrap();
        if (t == v) {
          uncons(s);
          return {t};
        }
        return {error<typename stream_trait<Stream>::value_type>(
            error_type::unexpected, t)};
      }

    private:
      V v;
    };

    template <typename V>
    struct is_parser<token_parser<V>> : std::true_type {};

    template <typename V>
    token_parser<V> token(V&& v) {
      return token_parser<V>(std::forward<V>(v));
    }

  } // namespace combix
} // namespace coco

#endif
