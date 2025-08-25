cmake_minimum_required(VERSION 3.20)
project(c10k_Echo_Server)
project(c10k_Echo_Client)
project(os_call_test)

set(CMAKE_CXX_EXTENSIONS 20)

set(TEST OFF)

if(UNIX)
    add_executable(c10k_Echo_Server "server_linux.cpp")
    add_executable(c10k_Echo_Client "client_linux.cpp")
elseif(WIN32)
    add_executable(c10k_Echo_Server "server_windows.cpp")
    add_executable(c10k_Echo_Client "client_windows.cpp")
endif()

if(TEST)
add_executable(os_call_test "test.cpp")
endif()