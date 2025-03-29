cmake_minimum_required(VERSION 3.10)
project(http_server)

set(CMAKE_CXX_STANDARD 17)

# Boost 라이브러리 찾기
find_package(Boost CONFIG REQUIRED)

# include 디렉토리 경로 추가
include_directories(${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)

# 실행 파일 추가
add_executable(http_server src/main.cpp src/server.cpp)

# Boost 라이브러리 링크
target_link_libraries(http_server ${Boost_LIBRARIES})

# Windows 소켓 라이브러리 추가
target_link_libraries(http_server ws2_32 mswsock)
