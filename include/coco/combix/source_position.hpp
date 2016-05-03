#ifndef COCO_COMBIX_SOURCE_POSITION_HPP_
#define COCO_COMBIX_SOURCE_POSITION_HPP_

namespace coco {
  namespace combix {

    struct source_position {
      constexpr explicit source_position() = default;
      constexpr source_position(std::size_t line, std::size_t column)
          : line(line), column(column) {}

      std::size_t line = 1;
      std::size_t column = 0;
    };

    constexpr bool operator==(source_position const& lhs,
                              source_position const& rhs) {
      return lhs.line == rhs.line && lhs.column == rhs.column;
    }

    constexpr bool operator!=(source_position const& lhs,
                              source_position const& rhs) {
      return !(lhs == rhs);
    }

    constexpr bool operator<(source_position const& lhs,
                             source_position const& rhs) {
      return lhs.line < rhs.line ||
             (lhs.line == rhs.line && lhs.column < rhs.column);
    }

    constexpr bool operator>(source_position const& lhs,
                              source_position const& rhs) {
      return rhs < lhs;
    }

    constexpr bool operator<=(source_position const& lhs,
                              source_position const& rhs) {
      return !(lhs > rhs);
    }

    constexpr bool operator>=(source_position const& lhs,
                              source_position const& rhs) {
      return !(lhs < rhs);
    }

  } // namespace combix
} // namespace coco

#endif
