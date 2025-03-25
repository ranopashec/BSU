cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 11)

# Добавляем Google Test
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Исходные файлы
set(SOURCES
        src/fibonacci.cpp
        src/palindrome.cpp
        src/linked_list.cpp
)

# Заголовочные файлы
include_directories(include)

# Тесты
set(TEST_SOURCES
        tests/test_fibonacci.cpp
        tests/test_palindrome.cpp
        tests/test_linked_list.cpp
)

add_executable(test_runner ${SOURCES} ${TEST_SOURCES})
target_link_libraries(test_runner ${GTEST_LIBRARIES} pthread)
