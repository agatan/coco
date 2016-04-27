file(GLOB_RECURSE FILES *.cpp)

add_library(coco
  ${FILES})

file(GLOB_RECURSE HEADERS ../include/*.hpp)

install(TARGETS coco
  EXPORT coco-export
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION include)
