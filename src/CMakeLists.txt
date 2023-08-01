cmake_minimum_required(VERSION 3.5)

project(print_ip LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(print_ip main.cpp)

install(TARGETS print_ip
    RUNTIME DESTINATION bin)


if (MSVC)                                                             
    target_compile_options(print_ip PRIVATE
        /W4
    )
else ()
    target_compile_options(print_ip PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

#set_target_properties(print_ip PROPERTIES COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
#set_target_properties(print_ip PROPERTIES LINK_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
