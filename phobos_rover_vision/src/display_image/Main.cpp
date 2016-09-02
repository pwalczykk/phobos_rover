#include <ros/ros.h>
#include "ImageWindow.hpp"

int main(int argc, char** argv){
    ros::init(argc, argv, "display_image");
    ros::NodeHandle nh("~");
    image_transport::ImageTransport it(nh);

    std::string camera_name;
    nh.param("camera_name", camera_name, (std::string)"cameraX");

    int width;
    int height;
    nh.param("width", width, 320);
    nh.param("height", height, 240);

    int x;
    int y;
    nh.param("x", x, 10);
    nh.param("y", y, 10);

    ImageWindow iw(&nh, &it, "/rover/" + camera_name + "/image", camera_name);
    iw.ResizeWindow(width, height);
    iw.MoveWindow(x, y);

    ros::spin();

    return 0;
}
