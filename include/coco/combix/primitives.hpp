#ifndef COCO_COMBIX_PRIMITIVES_HPP_
#define COCO_COMBIX_PRIMITIVES_HPP_

#include <coco/combix/error.hpp>
#include <coco/combix/parse_result.hpp>
#include <coco/combix/parser_traits.hpp>
#include <coco/combix/stream_traits.hpp>
#include <coco/combix/unused.hpp>
#include <coco/expected.hpp>

namespace coco {
  namespace combix {

    struct any_parser {
      template <typename Stream>
      using result_type = typename stream_traits<Stream>::value_type;

      template <typename Stream>
      parse_result<result_type<Stream>, Stream> operator()(Stream& s) const {
        auto res = uncons(s);
        if (res) {
          return *res;
        }
        res.unwrap_error().set_expected(expected_info<Stream>());
        return {res.unwrap_error()};
      }

      template <typename S>
      expected_list<typename stream_traits<S>::value_type> expected_info()
          const {
        return expected_list<typename stream_traits<S>::value_type>{
            error_info<typename stream_traits<S>::value_type>("any input")};
      }
    };

    any_parser any();

    template <typename F>
    struct satisfy_parser {
      template <typename Stream>
      using result_type = typename stream_traits<Stream>::value_type;

      satisfy_parser(F&& f) : f(std::move(f)) {}
      satisfy_parser(F const& f) : f(f) {}

      template <typename Stream>
      parse_result<result_type<Stream>, Stream> operator()(Stream& s) const {
        auto res = peek(s);
        if (!res) {
          return {res.unwrap_error()};
        }
        auto token = res.unwrap();
        if (f(token)) {
          uncons(s);
          return token;
        }
        using value_type = typename stream_traits<Stream>::value_type;
        return parse_error<value_type>(error_info<value_type>(token));
      }

      template <typename S>
      expected_list<typename stream_traits<S>::value_type> expected_info()
          const {
        return expected_list<typename stream_traits<S>::value_type>{};
      }

     private:
      F f;
    };

    template <typename F>
    satisfy_parser<std::decay_t<F>> satisfy(F&& f) {
      return satisfy_parser<std::decay_t<F>>(std::forward<F>(f));
    }

    template <typename V>
    struct token_parser {
      template <typename Stream>
      using result_type = typename stream_traits<Stream>::value_type;

      token_parser(V v) : v(std::move(v)) {}

      template <typename Stream>
      parse_result<result_type<Stream>, Stream> operator()(Stream& s) const {
        auto res = peek(s);
        if (!res) {
          return {res.unwrap_error()};
        }
        auto t = res.unwrap();
        if (t == v) {
          uncons(s);
          return {t};
        }
        using value_type = typename stream_traits<Stream>::value_type;
        return parse_error<value_type>(error_info<value_type>(t),
                                       expected_info<Stream>());
      }

      template <typename S>
      expected_list<typename stream_traits<S>::value_type> expected_info()
          const {
        return expected_list<typename stream_traits<S>::value_type>{
            error_info<typename stream_traits<S>::value_type>(v)};
      }

    private:
      V v;
    };

    template <typename V>
    token_parser<std::decay_t<V>> token(V&& v) {
      return {std::forward<V>(v)};
    }

    struct eof_parser {
      template <typename Stream>
      parse_result<unused, Stream> operator()(Stream& s) const {
        if (is_eof(s)) {
          return unused();
        }
        auto t = peek(s).unwrap();
        return parse_error<typename stream_traits<Stream>::value_type>(
            t, expected_info<Stream>());
      }

      template <typename Stream>
      expected_list<typename stream_traits<Stream>::value_type> expected_info()
          const {
        return expected_list<typename stream_traits<Stream>::value_type>(
            std::string{"end of input"});
      }
    };

    eof_parser eof();

  } // namespace combix
} // namespace coco

#endif
