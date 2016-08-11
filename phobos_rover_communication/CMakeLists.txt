cmake_minimum_required(VERSION 2.8.3)
project(phobos_rover_communication)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

find_package(catkin REQUIRED COMPONENTS
    roscpp
)

catkin_package()

add_executable(uart_433_rover_transceiver
    src/uart_433_rover_transceiver/Main.cpp)
target_link_libraries(uart_433_rover_transceiver ${catkin_LIBRARIES})
