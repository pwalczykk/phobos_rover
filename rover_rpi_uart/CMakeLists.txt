cmake_minimum_required(VERSION 2.8.3)
project(rover_rpi_uart)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

find_package(catkin REQUIRED COMPONENTS
    roscpp
)

include_directories(${catkin_INCLUDE_DIRS} ${roscpp_INCLUDE_DIRS})

catkin_package()

add_executable(uart_transmiter
    src/uart_transmiter/Main.cpp)
target_link_libraries(uart_transmiter ${catkin_LIBRARIES})

add_executable(uart_reciver
    src/uart_reciver/Main.cpp)
target_link_libraries(uart_reciver ${catkin_LIBRARIES})
