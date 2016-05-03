#ifndef COCO_COMBIX_POSITION_UPDATER_HPP_
#define COCO_COMBIX_POSITION_UPDATER_HPP_

#include <coco/combix/source_position.hpp>
#include <coco/combix/offset_position.hpp>

namespace coco {
  namespace combix {

    template <typename Value, typename Position>
    struct position_updater;

    template <>
    struct position_updater<char, source_position> {
      void operator()(char c, source_position& pos) {
        if (c == '\n') {
          pos.line++;
          pos.column = 1;
        } else {
          pos.column++;
        }
      }
    };

    template <typename Value>
    struct position_updater<Value, offset_position> {
      void operator()(Value const&, offset_position& pos) {
        pos.offset++;
      }
    };

  } // namespace combix
} // namespace coco

#endif
