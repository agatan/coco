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
      parse_result<std::string, Stream> parse(Stream& s) const {
        auto saved = save(s);
        std::string res;
        bool consumed = false;
        for (auto c : str) {
          auto chr = uncons(s);
          if (chr) {
            if (c == *chr) {
              res += *chr;
              consumed = true;
            } else {
              restore(s, std::move(saved));
              if (consumed) {
                auto err = parse_error<Stream>(make_unexpected<Stream>(*chr));
                err.add_error(make_expected<Stream>(str));
                err.consumed(true);
                return err;
              } else {
                return parse_error<Stream>();
              }
            }
          } else {
            restore(s, std::move(saved));
            chr.unwrap_error().consumed(consumed);
            return chr.unwrap_error();
          }
        }
        return res;
      }

      template <typename Stream>
      void add_error(parse_error<Stream>& err) const {
        err.add_error(make_expected<Stream>(str));
      }

    private:
      std::string str;
    };

    inline string_parser string(std::string const& s) {
      return string_parser(s);
    }

    inline string_parser string(std::string&& s) {
      return string_parser(std::move(s));
    }

  } // namespace combix
} // namespace coco

#endif
