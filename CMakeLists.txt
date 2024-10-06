cmake_minimum_required(VERSION 3.27)
project(working)

set(CMAKE_CXX_STANDARD 14)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
# include_directories(${PROJECT_SOURCE_DIR}/include/BigInteger)
enable_testing()

add_executable(hvtraceroute
        main.cpp
        tracing.h
        tracing.cpp)

# Создаем исполняемый файл для тестов
add_executable(my_tests tests/tests.cpp tracing.cpp)
target_link_libraries(my_tests ${GTEST_LIBRARIES} pthread)

# Добавляем тест
add_test(NAME MyTest COMMAND my_tests)

install(TARGETS hvtraceroute DESTINATION /usr/local/bin)
