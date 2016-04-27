#ifndef COCO_COMBIX_ERROR_HPP_
#define COCO_COMBIX_ERROR_HPP_

#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <list>

#include <boost/variant.hpp>

#include <coco/expected.hpp>

namespace coco {
  namespace combix {

    template <typename T>
    struct error_info {
      error_info() = default;
      error_info(T const& t): value_(t) {}
      error_info(std::string const& s): value_(s) {}

      boost::variant<T, std::string> value() const {
        return value_;
      }

    private:
        boost::variant<T, std::string> value_;
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, error_info<T> const& info) {
      struct visitor {
        std::ostream& operator()(T const& t) const {
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

    template <typename T>
    struct expected_list {
      expected_list() = default;

      explicit expected_list(error_info<T> const& info, bool nullable = false)
          : expected_{info}, nullable_(nullable) {}

      explicit expected_list(std::list<error_info<T>> const& es, bool nullable = false)
          : expected_(std::begin(es), std::end(es)), nullable_(nullable) {}

      using iterator_type = typename std::list<error_info<T>>::const_iterator;

      iterator_type begin() const {
        return expected_.begin();
      }

      iterator_type end() const {
        return expected_.end();
      }

      bool empty() const {
        return expected_.empty();
      }

      void push_back(error_info<T> const& info) {
        expected_.push_back(info);
      }

      void push_front(error_info<T> const& info) {
        expected_.push_front(info);
      }

      bool nullable() const {
        return nullable_;
      }

      bool nullable(bool n) {
        nullable_ = n;
      }

      void merge(expected_list const& rhs) {
        nullable_ = nullable_ && rhs.nullable();
        expected_.insert(expected_.end(), rhs.begin(), rhs.end());
      }

     private:
      std::list<error_info<T>> expected_;
      bool nullable_ = false;
    };

    template <typename T>
    struct parse_error {
      parse_error() = default;
      parse_error(error_info<T> const& unexpected) : unexpected_(unexpected) {}

      template <typename Expects>
      parse_error(error_info<T> const& unexpected, Expects const& expected)
          : unexpected_(unexpected),
            expected_(expected) {}

      ~parse_error() = default;

      expected_list<T> const& expected() const {
        return expected_;
      }

      boost::optional<error_info<T>> const& unexpected() const {
        return unexpected_;
      }

      boost::optional<std::string> const& message() const {
        return message_;
      }

      void add_expected(error_info<T> const& info) {
        expected_.push_back(info);
      }

      void set_expected(expected_list<T>&& infos) {
        expected_ = std::move(infos);
      }

      void set_unexpected(error_info<T> const& info) {
        unexpected_ = info;
      }

      void set_message(std::string const& msg) {
        message_ = msg;
      }

     private:
      boost::optional<error_info<T>> unexpected_;
      expected_list<T> expected_;
      boost::optional<std::string> message_;
    };

    template <typename T>
    std::string to_string(parse_error<T> const& err) {
      std::stringstream ss;
      if (err.message()) {
        ss << *(err.message()) << ": ";
      }
      if (err.unexpected()) {
        ss << "unexpected " << *(err.unexpected()) << ": ";
      }
      if (!err.expected().empty()) {
        ss << "expected ";
        for (auto it = std::begin(err.expected()), e = std::end(err.expected());
             it != e; ++it) {
          if (it != std::begin(err.expected())) {
            ss << ", ";
          }
          ss << *it;
        }
      }
      return ss.str();
    }

    template <typename T>
    parse_error<T> end_of_input() {
      parse_error<T> err;
      err.set_unexpected(error_info<T>("end of input"));
      return err;
    }

    template <typename T>
    parse_error<T> unexpected(error_info<T>&& info) {
      parse_error<T> err;
      err.set_unexpected(std::move(info));
      return err;
    }

  } // namespace combix
} // namespace coco

#endif
