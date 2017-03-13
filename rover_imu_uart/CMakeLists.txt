cmake_minimum_required(VERSION 2.8.3)
project(rover_imu_uart)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

find_package(catkin REQUIRED COMPONENTS
    roscpp
    message_generation
)

include_directories(${catkin_INCLUDE_DIRS} ${roscpp_INCLUDE_DIRS})

add_message_files(
    FILES
    WheelsVel2.msg
    ArmPose5.msg
)

generate_messages(
    DEPENDENCIES
    std_msgs
)

catkin_package(
    CATKIN_DEPENDS message_runtime
)


add_executable(uart_imu_receiver
    src/uart_imu_receiver/Main.cpp)
target_link_libraries(uart_imu_receiver ${catkin_LIBRARIES})
