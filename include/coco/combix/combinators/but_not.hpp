#ifndef COCO_COMBIX_COMBINATORS_BUT_NOT_HPP_
#define COCO_COMBIX_COMBINATORS_BUT_NOT_HPP_

#include <coco/combix/parse_result.hpp>
#include <coco/combix/stream_traits.hpp>
#include <coco/combix/parser_traits.hpp>

namespace coco {
  namespace combix {

    template <typename P, typename Not>
    struct but_not_parser {
      but_not_parser(P const& p, Not const& n) : parser(p), not_(n) {}
      but_not_parser(P&& p, Not&& n)
          : parser(std::move(p)), not_(std::move(n)) {}

      template <typename Stream>
      parse_result<parse_result_of_t<P, Stream>, Stream>
      operator()(Stream& s) const {
        auto saved = save(s);
        auto saved_for_not = save(s);
        auto res = parse(parser, s);
        if (!res) {
          return res.unwrap_error();
        }
        std::cerr << std::string(saved_for_not.begin(), saved_for_not.end()) << std::endl;
        auto not_res = parse(not_, saved_for_not);
        std::cerr << std::string(saved_for_not.begin(), saved_for_not.end()) << std::endl;
        if (not_res) {
          restore(s, std::move(saved));
          parse_error<typename stream_traits<Stream>::value_type> err;
          auto unexpected_list =
              coco::combix::expected_info<Stream>(not_);
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
        return res.unwrap();
      }

      template <typename Stream>
      expected_list<typename stream_traits<Stream>::value_type> expected_info()
          const {
        return coco::combix::expected_info<Stream>(parser);
      }

     private:
      P parser;
      Not not_;
    };

    template <typename P, typename Not>
    but_not_parser<std::decay_t<P>, std::decay_t<Not>>
    but_not(P&& p, Not&& n) {
      return but_not_parser<std::decay_t<P>, std::decay_t<Not>>(
          std::forward<P>(p), std::forward<Not>(n));
    }

  } // namespace combix
} // namespace coco

#endif
