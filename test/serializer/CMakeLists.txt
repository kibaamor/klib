set(TARGET_NAME "serializer")
add_executable(${TARGET_NAME} main.cpp ../doctest.h)
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "test")

target_link_libraries(${TARGET_NAME}
    ${PROJECT_NAME}
)

add_test(NAME serializer_test COMMAND $<TARGET_FILE:${TARGET_NAME}>)
