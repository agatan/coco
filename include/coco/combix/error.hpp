#ifndef COCO_COMBIX_ERROR_HPP_
#define COCO_COMBIX_ERROR_HPP_

#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <deque>
#include <vector>
#include <algorithm>

#include <boost/variant.hpp>

#include <coco/expected.hpp>
#include <coco/combix/stream_traits.hpp>

namespace coco {
  namespace combix {

    template <typename Stream>
    struct error_info {
      using value_type = stream_value_of_t<Stream>;

      error_info() = default;
      error_info(value_type const& t): value_(t) {}
      error_info(std::string const& s): value_(s) {}
      error_info(char const* s) : value_(std::string(s)) {}

      boost::variant<value_type, std::string> value() const {
        return value_;
      }

    private:
        boost::variant<value_type, std::string> value_;
    };

    template <typename Stream>
    bool operator==(error_info<Stream> const& lhs,
                    error_info<Stream> const& rhs) {
      return lhs.value() == rhs.value();
    }

    template <typename Stream>
    bool operator!=(error_info<Stream> const& lhs,
                    error_info<Stream> const& rhs) {
      return !(lhs == rhs);
    }

    template <typename Stream>
    std::ostream& operator<<(std::ostream& os, error_info<Stream> const& info) {
      struct visitor {
        std::ostream& operator()(typename Stream::value_type const& t) const {
          return os << '"' << t << '"';
        }

        std::ostream& operator()(std::string const& s) const {
          return os << s;
        }

        std::ostream& os;
      };

      auto const& v = info.value();
      return boost::apply_visitor(visitor{os}, v);
    }

    // error represents an error of parsing;
    // like `unexpected` token, `expected` tokens or `message`.
    enum class error_kind {
      unexpected,
      expected,
      message,
    };

    template <typename Stream>
    struct error {
      error() = default;
      error(error_kind k, error_info<Stream> const& i) : kind_(k), info_(i) {}

      error_kind kind() const {
        return kind_;
      }

      error_info<Stream> info() const {
        return info_;
      }
    private:
      error_kind kind_;
      error_info<Stream> info_;
    };

    template <typename Stream>
    bool operator==(error<Stream> const& lhs, error<Stream> const& rhs) {
      if (lhs.kind() != rhs.kind()) {
        return false;
      }
      return lhs.info() == rhs.info();
    }

    template <typename Stream>
    bool operator!=(error<Stream> const& lhs, error<Stream> const& rhs) {
      return !(lhs == rhs);
    }

    template <typename Stream>
    error<Stream> make_unexpected(error_info<Stream>&& info) {
      return error<Stream>(error_kind::unexpected, std::move(info));
    }

    template <typename Stream>
    error<Stream> make_expected(error_info<Stream>&& info) {
      return error<Stream>(error_kind::expected, std::move(info));
    }

  } // namespace combix
} // namespace coco

#endif
