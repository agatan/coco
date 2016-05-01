#ifndef COCO_COMBIX_COMBINATORS_EXPECTED_HPP_
#define COCO_COMBIX_COMBINATORS_EXPECTED_HPP_

#include <string>
#include <type_traits>

#include <coco/combix/error.hpp>
#include <coco/combix/parse_result.hpp>
#include <coco/combix/stream_traits.hpp>
#include <coco/combix/parser_traits.hpp>

namespace coco {
  namespace combix {

    template <typename P>
    struct expected_parser {
      explicit expected_parser(P const& p, std::string&& msg)
          : parser(p), msg(std::move(msg)) {}
      explicit expected_parser(P&& p, std::string&& msg)
          : parser(std::move(p)), msg(std::move(msg)) {}

      template <typename Stream>
      parse_result<parse_result_of_t<P, Stream>, Stream>
      parse(Stream& s) const {
        auto res = parser.parse(s);
        if (res) {
          return res;
        }
        return res.unwrap_error();
      }

      template <typename Stream>
      void add_error(parse_error<Stream>& err) const {
        auto prev_errs = err.errors().size();
        parser.add_error(err);
        std::size_t i = 0;
        auto it = std::remove_if(err.errors().begin(), err.errors().end(),
                                 [&](auto const& e) {
                                   if (i < prev_errs) {
                                     i++;
                                     return false;
                                   } else {
                                     return e.kind() == error_kind::expected;
                                   }
                                 });
        err.errors().erase(it, err.errors().end());
        err.add_error(make_expected<Stream>(msg));
      }

    private:
      P parser;
      std::string msg;
    };

    template <typename P>
    expected_parser<std::decay_t<P>> expected(P&& parser, std::string msg) {
      return expected_parser<std::decay_t<P>>(std::forward<P>(parser),
                                              std::move(msg));
    }

  } // namespace combix
} // namespace coco

#endif
