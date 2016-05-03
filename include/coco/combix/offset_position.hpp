#ifndef COCO_COMBIX_OFFSET_POSITION_HPP_
#define COCO_COMBIX_OFFSET_POSITION_HPP_

namespace coco {
  namespace combix {

    struct offset_position {
      explicit offset_position() = default;
      explicit offset_position(std::size_t ofs) : offset(ofs) {}

      std::size_t offset;
    };

  } // namespace combix
} // namespace coco

#endif
