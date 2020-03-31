add_executable(variant main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../doctest.h)
target_link_libraries(variant ${PROJECT_NAME})
set_property(TARGET variant PROPERTY FOLDER "test")
add_test(NAME test_variant COMMAND $<TARGET_FILE:variant>)
