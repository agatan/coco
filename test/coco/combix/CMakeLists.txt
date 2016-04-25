add_executable(stream_test stream_test.cpp)
add_test(NAME stream_test COMMAND $<TARGET_FILE:stream_test>)
target_link_libraries(stream_test
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

add_executable(primitives_test primitives_test.cpp)
add_test(NAME primitives_test COMMAND $<TARGET_FILE:primitives_test>)
target_link_libraries(primitives_test
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

add_executable(combinators_test combinators_test.cpp)
add_test(NAME combinators_test COMMAND $<TARGET_FILE:combinators_test>)
target_link_libraries(combinators_test
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

add_executable(parser_test parser_test.cpp)
add_test(NAME parser_test COMMAND $<TARGET_FILE:parser_test>)
target_link_libraries(parser_test
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )
