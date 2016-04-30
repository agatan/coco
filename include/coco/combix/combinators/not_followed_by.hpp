#ifndef COCO_COMBIX_COMBINATORS_NOT_FOLLOWED_BY_HPP_
#define COCO_COMBIX_COMBINATORS_NOT_FOLLOWED_BY_HPP_

#include <sstream>

#include <coco/combix/stream_traits.hpp>
#include <coco/combix/parser_traits.hpp>
#include <coco/combix/combinators/try_.hpp>

namespace coco {
  namespace combix {

    template <typename P, typename Not>
    struct not_followed_by_parser {
      not_followed_by_parser(P const& p, Not const& n)
          : parser(p), not_followed(n) {}
      not_followed_by_parser(P&& p, Not&& n)
          : parser(std::move(p)), not_followed(std::move(n)) {}

      template <typename Stream>
      parse_result<parse_result_of_t<P, Stream>, Stream>
      operator()(Stream& s) const {
        auto saved = save(s);
        auto res = parse(parser, s);
        if (!res) {
          return res.unwrap_error();
        }
        auto saved_main = save(s);
        auto follow = parse(not_followed, s);
        if (follow) {
          restore(s, std::move(saved));
          parse_error<typename stream_traits<Stream>::value_type> err;
          auto unexpected_list =
              coco::combix::expected_info<Stream>(not_followed);
          std::stringstream ss;
          for (auto it = std::begin(unexpected_list),
                    e = std::end(unexpected_list);
               it != e; ++it) {
            if (it != std::begin(unexpected_list)) {
              ss << ", ";
            }
            ss << *it;
          }
          err.set_unexpected(ss.str());
          return err;
        }
        restore(s, std::move(saved_main));
        return res.unwrap();
      }

      template <typename Stream>
      expected_list<typename stream_traits<Stream>::value_type> expected_info()
          const {
        return expected_info<Stream>(parser);
      }

     private:
      P parser;
      Not not_followed;
    };

    template <typename P, typename Not>
    not_followed_by_parser<std::decay_t<P>, std::decay_t<Not>>
    not_followed_by(P&& p, Not&& n) {
      return not_followed_by_parser<std::decay_t<P>, std::decay_t<Not>>(
          std::forward<P>(p), std::forward<Not>(n));
    }

  } // namespace combix
} // namespace coco

#endif