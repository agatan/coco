#ifndef COCO_COMBIX_OFFSET_POSITION_HPP_
#define COCO_COMBIX_OFFSET_POSITION_HPP_

namespace coco {
  namespace combix {

    struct offset_position {
      constexpr explicit offset_position() = default;
      constexpr explicit offset_position(std::size_t ofs) : offset(ofs) {}

      std::size_t offset = 0;
    };

    constexpr bool operator==(offset_position const& lhs,
                              offset_position const& rhs) {
      return lhs.offset == rhs.offset;
    }

    constexpr bool operator!=(offset_position const& lhs,
                              offset_position const& rhs) {
      return !(lhs == rhs);
    }

    constexpr bool operator<(offset_position const& lhs,
                             offset_position const& rhs) {
      return lhs.offset < rhs.offset;
    }

    constexpr bool operator>(offset_position const& lhs,
                              offset_position const& rhs) {
      return rhs < lhs;
    }

    constexpr bool operator<=(offset_position const& lhs,
                              offset_position const& rhs) {
      return !(lhs > rhs);
    }

    constexpr bool operator>=(offset_position const& lhs,
                              offset_position const& rhs) {
      return !(lhs < rhs);
    }

  } // namespace combix
} // namespace coco

#endif
