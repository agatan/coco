#ifndef COCO_COMBIX_POSITIONED_STREAM_HPP_
#define COCO_COMBIX_POSITIONED_STREAM_HPP_

#include <coco/combix/position_updater.hpp>

namespace coco {
  namespace combix {

    template <typename Stream, typename Position,
              typename Updater = position_updater<
                  typename stream_traits<Stream>::value_type, Position>>
    struct positioned_stream {
      using value_type = typename stream_traits<Stream>::value_type;
      using iterator_type = typename stream_traits<Stream>::iterator_type;
      using position_type = Position;

      explicit positioned_stream(Stream&& s, Updater u = Updater{},
                                 Position p = Position{})
          : updater_(std::move(u)),
            stream_(std::move(s)),
            position_(std::move(p)) {}

      positioned_stream(positioned_stream&&) = default;
      positioned_stream(positioned_stream const&) = default;
      positioned_stream& operator=(positioned_stream const&) = default;
      positioned_stream& operator=(positioned_stream&&) = default;

      iterator_type begin() const {
        using std::begin;
        return begin(stream_);
      }
      iterator_type end() const {
        using std::end;
        return end(stream_);
      }

      positioned_stream save() const {
        return *this;
      }

      void restore(positioned_stream&& ps) {
        *this = std::move(ps);
      }

      coco::expected<value_type, parse_error<positioned_stream>> uncons() {
        auto res = stream_traits<Stream>::uncons(stream_);
        if (res) {
          updater_(*res, position_);
          return *res;
        }
        return end_of_input(*this);
      }

      bool is_eof() const {
        return stream_traits<Stream>::is_eof(stream_);
      }

      position_type const& position() const & {
        return position_;
      }

      position_type position() && {
        return std::move(position_);
      }

     private:
      Updater updater_;
      Stream stream_;
      Position position_;
    };

    template <typename Position, typename Stream>
    positioned_stream<std::decay_t<Stream>, Position> make_positioned(
        Stream&& s) {
      return positioned_stream<std::decay_t<Stream>, Position>(
          std::forward<Stream>(s));
    }

  } // namespace combix
} // namespace coco

#endif
