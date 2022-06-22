
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(activenotes)
add_subdirectory(channel-users)
add_subdirectory(wopn-file)

add_library(Catch-objects OBJECT "common/catch_main.cpp")
target_include_directories(Catch-objects PRIVATE "common")
