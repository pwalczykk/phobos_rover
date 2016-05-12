cmake_minimum_required(VERSION 2.8.3)
project(rover_vision)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    image_transport
    cv_bridge
)

find_package(OpenCV 3.0.0 REQUIRED)
catkin_package()

include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS})

add_executable(titanum_camera_driver
    src/titanum_camera_driver/Main.cpp)
target_link_libraries(titanum_camera_driver ${Boost_LIBRARIES} ${OpenCV_LIBRARIES} ${catkin_LIBRARIES})
