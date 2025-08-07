cmake_minimum_required(VERSION 3.16)
project(baekjoon)

set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_DEBUG} -pg" CACHE STRING "Flags used by the C++ compiler for Profile builds." FORCE)
# CMAKE_EXE_LINKER_FLAGS_PROFILE 변수에 -pg 플래그를 추가합니다.
set(CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -pg" CACHE STRING "Flags used by the linker for Profile builds." FORCE)

add_executable(baekjoon "가운데를_말해요.cpp")
