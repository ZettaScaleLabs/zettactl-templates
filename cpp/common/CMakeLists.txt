cmake_minimum_required(VERSION 3.20)
project(your_workspace_name LANGUAGES C CXX)

find_package(zenohc REQUIRED)
find_package(zenohcxx REQUIRED)
#find_package(zenohpico REQUIRED)

add_custom_target(src ALL)

function(add_platform_libraries target)
	if(APPLE)
		find_library(FFoundation Foundation)
		find_library(FSecurity Security)
		target_link_libraries(${target} PUBLIC ${FFoundation} ${FSecurity})
	elseif(UNIX)
		target_link_libraries(${target} PUBLIC rt pthread m dl)
	elseif(WIN32)
		target_link_libraries(${target} PUBLIC ws2_32 crypt32 secur32 bcrypt ncrypt userenv ntdll iphlpapi runtimeobject)
	endif()
endfunction()

function(add_zenoh_file file mode lib)
	get_filename_component(filename ${file} NAME_WE)
	set(target ${filename}_${mode})
	add_executable(${target} EXCLUDE_FROM_ALL ${file})
	set_target_properties(${target} PROPERTIES 
		OUTPUT_NAME ${filename}
		RUNTIME_OUTPUT_DIRECTORY ${mode})
	add_dependencies(src ${target})
	add_dependencies(${target} ${lib})
    target_link_libraries(${target} PUBLIC ${lib})
	set_property(TARGET ${target} PROPERTY LANGUAGE CXX)
	set_property(TARGET ${target} PROPERTY CXX_STANDARD 17)
	add_platform_libraries(${target})
endfunction()

function(add_zenoh_files glob mode lib)
	file(GLOB files ${glob})
	foreach(file ${files})
		add_zenoh_file(${file} ${mode} ${lib})
	endforeach()
endfunction()

# zenohcxx examples compiled with C++ compiler with zenohc
add_zenoh_files("${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" zenohc zenohcxx::zenohc::lib)

# zenohcxx examples compiled with C++ compiler with zenohpico
# add_zenoh_files("${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" zenohpico zenohcxx::zenohpico)