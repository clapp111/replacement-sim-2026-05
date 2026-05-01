cmake_minimum_required(VERSION 3.20)

project(replacement_sim
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 지정된 소스 파일을 사용하여 프로젝트에 라이브러리를 추가한다.
add_library(replacement_sim
    src/replacement_sim/trace_parser.cpp
)

# 대상에 포함 디렉토리를 추가하여 헤더 파일을 찾을 수 있도록 한다.
target_include_directories(replacement_sim
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

# 현재 디렉토리와 하위 디렉토리에 대한 테스트를 허용한다.
enable_testing()

# 공통 테스트 실행 파일. 새 기능 테스트는 tests/*_tests.cpp로 추가한다.
add_executable(replacement_sim_tests
    tests/test_main.cpp
    tests/trace_parser_tests.cpp
)

target_link_libraries(replacement_sim_tests
    PRIVATE
        replacement_sim
)

# ctest로 실행할 테스트를 프로젝트에 추가한다.
add_test(
    NAME replacement_sim_tests
    COMMAND replacement_sim_tests
)
