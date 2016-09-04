cmake_minimum_required(VERSION 2.8.3)
project(phobos_rover_vision)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    tf
    cv_bridge
    image_transport
    phobos_shared
)

find_package(OpenCV 3.0.0 REQUIRED)

catkin_package()

include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS})

add_executable(tf_broadcaster
    src/tf_broadcaster/Main.cpp)
target_link_libraries(tf_broadcaster ${catkin_LIBRARIES})

add_executable(ip_camera_driver
    src/ip_camera_driver/Main.cpp)
target_link_libraries(ip_camera_driver ${Boost_LIBRARIES} ${OpenCV_LIBRARIES} ${catkin_LIBRARIES})

add_executable(titanum_camera_driver
    src/titanum_camera_driver/Main.cpp)
target_link_libraries(titanum_camera_driver ${Boost_LIBRARIES} ${OpenCV_LIBRARIES} ${catkin_LIBRARIES})

add_executable(display_image
    src/display_image/Main.cpp)
target_link_libraries(display_image ${Boost_LIBRARIES} ${OpenCV_LIBRARIES} ${catkin_LIBRARIES})

add_executable(cameras_synchronizer
    src/cameras_synchronizer/Main.cpp)
target_link_libraries(cameras_synchronizer ${catkin_LIBRARIES})

add_executable(cam_info_publisher
    src/cam_info_publisher/Main.cpp)
target_link_libraries(cam_info_publisher ${catkin_LIBRARIES})
