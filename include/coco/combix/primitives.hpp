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
      parse_result<result_type<Stream>, Stream> parse(Stream& s) const {
        return uncons(s);
      }

      template <typename Stream>
      void add_error(parse_error<Stream>&) const {}
    };

    inline any_parser any() {
      return any_parser{};
    }

    template <typename F>
    struct satisfy_parser {
      template <typename Stream>
      using result_type = typename stream_traits<Stream>::value_type;

      satisfy_parser(F&& f) : f(std::move(f)) {}
      satisfy_parser(F const& f) : f(f) {}

      template <typename Stream>
      parse_result<result_type<Stream>, Stream> parse(Stream& s) const {
        auto saved = save(s);
        auto res = uncons(s);
        if (!res) {
          return {res.unwrap_error()};
        }
        auto token = res.unwrap();
        if (f(token)) {
          return token;
        }
        restore(s, std::move(saved));
        return parse_error<Stream>();
      }

      template <typename Stream>
      void add_error(parse_error<Stream>&) const {}

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
      parse_result<result_type<Stream>, Stream> parse(Stream& s) const {
        auto saved = save(s);
        auto res = uncons(s);
        if (!res) {
          return {res.unwrap_error()};
        }
        auto t = res.unwrap();
        if (t == v) {
          return {t};
        }
        restore(s, std::move(saved));
        return parse_error<Stream>();
      }

      template <typename Stream>
      void add_error(parse_error<Stream>& err) const {
        err.add_error(make_expected<Stream>(v));
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
      parse_result<unused, Stream> parse(Stream& s) const {
        if (is_eof(s)) {
          return unused();
        }
        return parse_error<Stream>();
      }

      template <typename Stream>
      void add_error(parse_error<Stream>& err) const {
        err.add_error(make_expected<Stream>("end of input"));
      }
    };

    inline eof_parser eof() {
      return eof_parser{};
    }

  } // namespace combix
} // namespace coco

#endif
