add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

function(add_subdir TARGET_NAME)
	add_subdirectory(${TARGET_NAME})
	set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "test")
endfunction(add_subdir)

add_subdir(serializer)
add_subdir(variant)
