#ifndef COCO_COMBIX_COMBINATORS_SKIP_SEQ_HPP_
#define COCO_COMBIX_COMBINATORS_SKIP_SEQ_HPP_

#include <coco/combix/combinators/skip.hpp>
#include <coco/combix/combinators/seq.hpp>

namespace coco {
  namespace combix {

    template <typename Skipper>
    struct skip_seq_adder {
      explicit skip_seq_adder(Skipper&& s) : skipper(std::move(s)) {}
      explicit skip_seq_adder(Skipper const& s) : skipper(s) {}

      template <typename... Args>
      auto operator()(Args&&... args) {
        return seq(skip(std::forward<Args>(args), skipper)...);
      }
    private:
      Skipper skipper;
    };

    template <typename Skipper>
    skip_seq_adder<std::decay_t<Skipper>> skip_seq(Skipper&& skipper) {
      return skip_seq_adder<std::decay_t<Skipper>>(
          std::forward<Skipper>(skipper));
    }

  } // namespace combix
} // namespace coco

#endif
