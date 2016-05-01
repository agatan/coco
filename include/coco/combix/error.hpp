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

    template <typename Stream>
    struct parse_error {
      parse_error() = default;
      explicit parse_error(error<Stream> const& err) : errors_({err}) {}

      void add_error(error<Stream>&& err) {
        // if `err` is not in errors_, push `err` into `errors_`
        if (std::all_of(errors_.cbegin(), errors_.cend(), [&](auto const& e) { return e != err; })) {
          errors_.emplace_back(std::move(err));
        }
      }

      void set_expected(error<Stream>&& err) {
        // remove all of `expected` message from `errors_`
        auto it = std::remove_if(
            errors_.begin(), errors_.end(),
            [](auto const& e) { return e.kind() == error_kind::expected; });
        errors_.erase(it, errors_.end());
        errors_.emplace_back(std::move(err));
      }

      void set_expected(error_info<Stream>&& err) {
        // remove all of `expected` message from `errors_`
        auto it = std::remove_if(
            errors_.begin(), errors_.end(),
            [](auto const& e) { return e.kind() == error_kind::expected; });
        errors_.erase(it, errors_.end());
        errors_.emplace_back(make_expected(std::move(err)));
      }

      bool consumed() const {
        return consumed_;
      }

      void consumed(bool v) {
        consumed_ = v;
      }

      std::deque<error<Stream>> const& errors() const {
        return errors_;
      }

      std::deque<error<Stream>>& errors() {
        return errors_;
      }

      void merge(parse_error&& rhs) {
        for (auto&& e : rhs.errors()) {
          add_error(std::move(e));
        }
      }

    private:
      std::deque<error<Stream>> errors_;
      bool consumed_ = false;
    };

    template <typename Stream>
    std::ostream& operator<<(std::ostream& os, parse_error<Stream> const& err) {
      std::vector<error<Stream>> unexpecteds;
      std::copy_if(err.errors().cbegin(), err.errors().cend(),
                   std::back_inserter(unexpecteds), [](auto const& e) {
                     return e.kind() == error_kind::unexpected;
                   });
      for (auto it = unexpecteds.cbegin(), e = unexpecteds.cend(); it != e;
           ++it) {
        if (it == unexpecteds.cbegin()) {
          os << "Unexpected ";
        } else {
          os << ", ";
        }
        os << it->info();
      }
      if (!unexpecteds.empty()) {
        os << "\n";
      }

      std::vector<error<Stream>> expecteds;
      std::copy_if(err.errors().cbegin(), err.errors().cend(),
                   std::back_inserter(expecteds), [](auto const& e) {
                     return e.kind() == error_kind::expected;
                   });
      for (auto it = expecteds.cbegin(), e = expecteds.cend(); it != e;
           ++it) {
        if (it == expecteds.cbegin()) {
          os << "Expected ";
        } else {
          os << ", ";
        }
        os << it->info();
      }
      if (!expecteds.empty()) {
        os << "\n";
      }

      std::vector<error<Stream>> messages;
      std::copy_if(err.errors().cbegin(), err.errors().cend(),
                   std::back_inserter(messages), [](auto const& e) {
                     return e.kind() == error_kind::message;
                   });
      for (auto&& msg: messages) {
        os << msg.info() << std::endl;
      }
      return os;
    }

    template <typename Stream>
    parse_error<Stream> end_of_input() {
      return parse_error<Stream>(make_unexpected<Stream>("end of input"));
    }

  } // namespace combix
} // namespace coco

#endif
