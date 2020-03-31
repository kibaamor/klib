add_executable(serializer main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../doctest.h)
target_link_libraries(serializer ${PROJECT_NAME})
set_property(TARGET serializer PROPERTY FOLDER "test")
add_test(NAME test_serializer COMMAND $<TARGET_FILE:serializer>)
