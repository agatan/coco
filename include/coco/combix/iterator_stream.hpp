#ifndef COCO_COMBIX_ITERATOR_STREAM_HPP_
#define COCO_COMBIX_ITERATOR_STREAM_HPP_

#include <iterator>

#include <coco/combix/error.hpp>
#include <coco/combix/stream_traits.hpp>
#include <coco/expected.hpp>

namespace coco {
  namespace combix {

    template <typename Iter>
    struct iterator_stream {
      using value_type = typename std::iterator_traits<Iter>::value_type;

      iterator_stream(Iter b, Iter e) : begin_(b), end_(e) {}
      ~iterator_stream() = default;

      iterator_stream(iterator_stream const&) = delete;
      iterator_stream(iterator_stream&&) = default;

      iterator_stream& operator=(iterator_stream const&) = delete;
      iterator_stream& operator=(iterator_stream&&) = default;

      Iter begin() const { return begin_; }
      Iter end() const { return end_; }

      iterator_stream save() const {
        return iterator_stream<Iter>(begin_, end_);
      }

      coco::expected<value_type, parse_error<value_type>> peek() {
        if (begin_ == end_) {
          return end_of_input<value_type>();
        }
        return *begin_;
      }
      coco::expected<value_type, parse_error<value_type>> uncons() {
        if (begin_ == end_) {
          return end_of_input<value_type>();
        }
        return *begin_++;
      }

    private:
      Iter begin_;
      Iter end_;
    };

    template <typename Iter>
    iterator_stream<Iter> iter_stream(Iter b, Iter e) {
      return { b, e };
    }

    template <typename Range>
    auto range_stream(Range const& range)
        -> iterator_stream<decltype(std::cbegin(range))> {
      return {std::cbegin(range), std::cend(range)};
    }

  } // namespace combix
} // namespace coco

#endif
