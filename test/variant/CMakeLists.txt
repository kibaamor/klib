file(GLOB SRC_LIST 
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h" 
	"${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
)

set(TARGET_NAME "variant")
add_executable(${TARGET_NAME} ${SRC_LIST})

target_link_libraries(${TARGET_NAME}
    klib
    doctest
)

add_test(NAME variant_test COMMAND $<TARGET_FILE:${TARGET_NAME}>)
