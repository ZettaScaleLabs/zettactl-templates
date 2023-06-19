cmake_minimum_required(VERSION 3.20)
project(your_workspace_name LANGUAGES C)

find_package(zenohc REQUIRED)
#find_package(zenohpico REQUIRED)

set(ZENOHC_BUILD_WITH_STATIC_LIB FALSE CACHE BOOL "Use static zenohc lib")

add_custom_target(src ALL)

file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

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

foreach(file ${files})
    get_filename_component(target ${file} NAME_WE)

    add_executable(${target} EXCLUDE_FROM_ALL ${file})
    add_dependencies(src ${target})
    add_platform_libraries(${target})

    if(ZENOHC_BUILD_WITH_STATIC_LIB)
        add_dependencies(${target} zenohc::static)
        target_link_libraries(${target} PRIVATE zenohc::static)
    else()
        add_dependencies(${target} zenohc::lib)
        target_link_libraries(${target} PRIVATE zenohc::lib)
        if(WIN32)
		    add_custom_command(TARGET ${target} POST_BUILD
		    	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
		    	COMMAND_EXPAND_LISTS
		    )
	    endif()
    endif()

    set_property(TARGET ${target} PROPERTY C_STANDARD 11)
endforeach()