cmake_minimum_required(VERSION 2.8.3)
project(phobos_rover_telemetry)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

find_package(catkin REQUIRED COMPONENTS
    roscpp
)

catkin_package()

add_executable(uart_433_rover_transmiter
    src/uart_433_rover_transmiter/Main.cpp)
target_link_libraries(uart_433_rover_transmiter ${catkin_LIBRARIES})

add_executable(wheels_odometry
    src/wheels_odometry/Main.cpp)
target_link_libraries(wheels_odometry ${catkin_LIBRARIES})
