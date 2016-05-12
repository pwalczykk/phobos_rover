cmake_minimum_required(VERSION 2.8.3)
project(rover_localization)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

find_package(catkin REQUIRED COMPONENTS
    roscpp
    rospy
)

catkin_package()

add_executable(traveled_path_publisher
    src/traveled_path_publisher/Main.cpp)
target_link_libraries(traveled_path_publisher ${catkin_LIBRARIES})
