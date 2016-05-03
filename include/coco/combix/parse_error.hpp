#ifndef COCO_COMBIX_PARSE_ERROR_HPP_
#define COCO_COMBIX_PARSE_ERROR_HPP_

#include <coco/combix/error.hpp>

namespace coco {
  namespace combix {

    namespace detail {
      struct has_position_impl {
        template <typename T>
        static auto check(T* t)
            -> decltype(std::declval<typename T::position_type>(), t->position(),
                        std::true_type{});

        template <typename>
        static auto check(...) -> std::false_type;
      };

      template <typename T>
      struct has_position : decltype(has_position_impl::check<T>(nullptr)) {};

      template <typename T>
      constexpr bool has_position_v = has_position<T>::value;

      enum class order {
        LT,
        EQ,
        GT
      };

      template <typename Stream>
      struct position_holder {
        using position_type = typename Stream::position_type;

        position_holder() = default;
        explicit position_holder(Stream const& s) : position_(s.position()) {}

        position_type const& position() const & {
          return position_;
        }

        position_type position() && {
          return std::move(position_);
        }

        void position(position_type const& pos) {
          position_ = pos;
        }

        order compare(position_holder const& rhs) const {
          if (position_ == rhs.position_) {
            return order::EQ;
          } else if (position_ < rhs.position_) {
            return order::LT;
          } else {
            return order::GT;
          }
        }

      private:
        position_type position_;
      };

      template <typename Stream>
      struct empty_stub {
        empty_stub() = default;
        explicit empty_stub(Stream const&) {}

        order compare(empty_stub const&) const {
          return order::EQ;
        }
      };

    } // namespace detail

    template <typename Stream>
    struct parse_error : public std::conditional_t<detail::has_position_v<Stream>,
                                          detail::position_holder<Stream>,
                                          detail::empty_stub<Stream>> {
      using base_type = std::conditional_t<detail::has_position_v<Stream>,
                                           detail::position_holder<Stream>,
                                           detail::empty_stub<Stream>>;

      parse_error() = default;
      explicit parse_error(Stream const& s) : base_type(s), errors_() {}
      explicit parse_error(Stream const& s, error<Stream> const& err)
          : base_type(s), errors_({err}) {}

      parse_error(parse_error const&) = default;
      parse_error(parse_error&&) = default;

      parse_error& operator=(parse_error const&) = default;
      parse_error& operator=(parse_error&&) = default;

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
        detail::order ord{base_type::compare(rhs)};
        switch (ord) {
        case detail::order::EQ:
          for (auto&& e : rhs.errors()) {
            add_error(std::move(e));
          }
          break;
        case detail::order::GT:
          break;
        case detail::order::LT:
          *this = std::move(rhs);
          break;
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
    parse_error<Stream> end_of_input(Stream const& stream) {
      return parse_error<Stream>(stream, make_unexpected<Stream>("end of input"));
    }

  } // namespace combix
} // namespace coco

#endif
