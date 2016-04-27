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
      operator()(Stream& s) const {
        auto res = parse(parser, s);
        if (res) {
          return res;
        }
        res.unwrap_error().set_expected(expected_info<Stream>());
        return res.unwrap_error();
      }

      template <typename Stream>
      expected_list<typename stream_traits<Stream>::value_type> expected_info()
          const {
        return expected_list<typename stream_traits<Stream>::value_type>(msg);
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
