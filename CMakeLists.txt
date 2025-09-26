# CMakeList.txt : CMake project for main, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.16)
project(DebuggableTemplate LANGUAGES CXX)

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

# CI11
FetchContent_Declare(
	CLI11
	GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
	GIT_TAG v2.3.2
)
FetchContent_MakeAvailable(cli11)

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

add_library(core_options
	src/logging.cpp
	src/config.cpp
)
target_include_directories(core_options PUBLIC include)
target_link_libraries(core_options PUBLIC spdlog::spdlog CLI11::CLI11)

# Add source to this project's executable.
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE core_options nlohmann_json::nlohmann_json httplib::httplib)

# Tests
# TODO: Add tests and install targets if needed.
enable_testing()
add_executable(tests test/test_basic.cpp)
target_link_libraries(tests PRIVATE core_options Catch2::Catch2WithMain)
add_test(NAME basic COMMAND tests)

add_custom_target(superclean
	COMMAND ${CMAKE_COMMAND} -E rm -rf
	"${CMAKE_BINARY_DIR}/CMakeFiles"
	"${CMAKE_BINARY_DIR}/CMakeCache.txt"
	"${CMAKE_BINARY_DIR}/core.dir"
	"${CMAKE_BINARY_DIR}/main.dir"
	"${CMAKE_BINARY_DIR}/tests.dir"
	"${CMAKE_BINARY_DIR}/Testing"
	"${CMAKE_BINARY_DIR}/Makefile"
	"${CMAKE_BINARY_DIR}/*.ninja"
	COMMENT "Cleaning all build artifacts except _deps"
)
