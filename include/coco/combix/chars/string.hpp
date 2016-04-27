#ifndef COCO_COMBIX_CHARS_STTRING_HPP_
#define COCO_COMBIX_CHARS_STTRING_HPP_

#include <string>

#include <coco/combix/parse_result.hpp>
#include <coco/combix/stream_traits.hpp>
#include <coco/combix/parser_traits.hpp>
#include <coco/combix/primitives.hpp>

namespace coco {
  namespace combix {

    struct string_parser {
      explicit string_parser(std::string const& s) : str(s) {}
      explicit string_parser(std::string&& s) : str(std::move(s)) {}

      template <typename Stream>
      parse_result<std::string, Stream> operator()(Stream& s) const {
        auto saved = save(s);
        std::string res;
        for (auto c : str) {
          auto chr = uncons(s);
          if (chr) {
            if (c == *chr) {
              res += *chr;
              continue;
            }
          }
          restore(s, std::move(saved));
          chr.unwrap_error().set_expected(expected_info<Stream>());
          return chr.unwrap_error();
        }
        return res;
      }

      template <typename Stream>
      expected_list<typename stream_traits<Stream>::value_type> expected_info()
          const {
        return expected_list<typename stream_traits<Stream>::value_type>(str);
      }

    private:
      std::string str;
    };

    string_parser string(std::string const& s);

    string_parser string(std::string&& s);

  } // namespace combix
} // namespace coco

#endif
