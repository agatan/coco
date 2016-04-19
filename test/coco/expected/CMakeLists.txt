add_executable(expected_test expected_test.cpp)
add_test(NAME expected_test COMMAND $<TARGET_FILE:expected_test>)
target_link_libraries(expected_test
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )
