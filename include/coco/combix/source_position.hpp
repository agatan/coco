#ifndef COCO_COMBIX_SOURCE_POSITION_HPP_
#define COCO_COMBIX_SOURCE_POSITION_HPP_

namespace coco {
  namespace combix {

    struct source_position {
      explicit source_position() = default;
      source_position(std::size_t line, std::size_t column)
          : line(line), column(column) {}

      std::size_t line = 1;
      std::size_t column = 0;
    };

  } // namespace combix
} // namespace coco

#endif
