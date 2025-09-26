# CMakeList.txt : CMake project for cpp, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.16)
project(DebuggableTemplate LANGUAGES CXX)

if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cpp PROPERTY CXX_STANDARD 20)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# spdlog
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.14.1
)
FetchContent_MakeAvailable(spdlog)

# cxxopts
FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG v3.2.0
)
FetchContent_MakeAvailable(cxxopts)

# nlohmann/json
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# cpp-httplib
FetchContent_Declare(
  httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_TAG v0.15.3
)
FetchContent_MakeAvailable(httplib)

# Catch2
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(Catch2)

# Add source to this project's executable.
add_executable(main src/main.cpp "include/inc1.h")
target_link_libraries(main PRIVATE spdlog::spdlog cxxopts nlohmann_json::nlohmann_json httplib::httplib)

# Tests
# TODO: Add tests and install targets if needed.
enable_testing()
add_executable(tests tests/test_basic.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
add_test(NAME basic COMMAND tests)
