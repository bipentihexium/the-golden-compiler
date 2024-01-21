cmake_minimum_required(VERSION 3.24)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(TheGoldenCompiler VERSION 0.1.0.0 LANGUAGES C CXX)

find_package(Boost COMPONENTS program_options REQUIRED)
find_package(LLVM COMPONENTS all REQUIRED)
execute_process(COMMAND llvm-config --libs all OUTPUT_VARIABLE llvm_libs)

configure_file(./src/cmake.hpp.in ${CMAKE_SOURCE_DIR}/src/cmake.hpp)
add_executable(tgc ./src/main.cpp ./src/parser/lex.cpp ./src/codegen.cpp)
target_compile_features(tgc PUBLIC cxx_std_20)
target_compile_options(tgc PUBLIC -Wall -Wextra -Wpedantic)
target_link_libraries(tgc PUBLIC ${Boost_LIBRARIES})
string(REPLACE " " ";" llvm_libs_list ${llvm_libs})
foreach(i ${llvm_libs_list})
	string(SUBSTRING ${i} 2 -1 i)
	string(STRIP ${i} i)
	target_link_libraries(tgc PUBLIC ${i})
endforeach()

add_library(tgcore ./src/corelib/corelib.cpp)
target_compile_options(tgcore PUBLIC -Wall -Wextra -Wpedantic)
