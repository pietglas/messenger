project(messenger_client)
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS "-Wall -Wextra -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

find_package(Threads)

add_library(client client.cpp)
add_library(transport local-echo-transport.cpp tcp-echo-transport.cpp)
add_executable(main main.cpp)

target_link_libraries(main client transport Threads::Threads)