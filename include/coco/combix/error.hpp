#ifndef COCO_COMBIX_ERROR_HPP_
#define COCO_COMBIX_ERROR_HPP_

#include <memory>
#include <string>
#include <vector>

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

    enum class error_type {
      unexpected,
      expected,
      other
    };

    template <typename T>
    struct error {
      error() = default;
      error(error_type t, error_info<T> const& i)
          : type_(t), info_(i) {}
      error(error_type t, std::string const& s)
          : error(t, error_info<T>(s)) {}

    private:
      error_type type_;
      error_info<T> info_;
    };

    template <typename T>
    error<T> end_of_input();

    template <typename T>
    struct parse_error {
      parse_error() = default;
      ~parse_error() = default;

      parse_error(error<T> const& err) : errors({err}) {}
      parse_error(error<T>&& err) : errors({err}) {}
      explicit parse_error(std::vector<error<T>> const& errs)
          : errors(errs) {}
      explicit parse_error(std::vector<error<T>>&& errs)
          : errors(std::move(errs)) {}

     private:
      std::vector<error<T>> errors;
    };

    template <typename T>
    error<T> end_of_input() {
      return error<T>(error_type::unexpected, "end of input");
    }

  } // namespace combix
} // namespace coco

#endif
