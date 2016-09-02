#include <ros/ros.h>
#include "ImageWindow.hpp"

int main(int argc, char** argv){
    ros::init(argc, argv, "display_image");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);

    int width1, width2, width3, width4;
    int height1, height2, height3, height4;

    int x1, x2, x3, x4;
    int y1, y2, y3, y4;

    nh.param("width1", width1, 320);
    nh.param("width2", width2, 320);
    nh.param("width3", width3, 320);
    nh.param("width4", width4, 320);

    nh.param("height1", height1, 240);
    nh.param("height2", height2, 240);
    nh.param("height3", height3, 240);
    nh.param("height4", height4, 240);

    nh.param("x1", x1, 10);
    nh.param("x2", x2, 330);
    nh.param("x3", x3, 10);
    nh.param("x4", x4, 330);

    nh.param("y1", y1, 10);
    nh.param("y2", y2, 10);
    nh.param("y3", y3, 250);
    nh.param("y4", y4, 250);

    ImageWindow iw1(&nh, &it, "/rover/camera1/image","win1");
    ImageWindow iw2(&nh, &it, "/rover/camera2/image","win2");
    ImageWindow iw3(&nh, &it, "/rover/camera3/image","win3");
    ImageWindow iw4(&nh, &it, "/rover/camera4/image","win4");

    iw1.ResizeWindow(width1, height1);
    iw2.ResizeWindow(width2, height2);
    iw3.ResizeWindow(width3, height3);
    iw4.ResizeWindow(width4, height4);

    iw1.MoveWindow(x1, y1);
    iw2.MoveWindow(x2, y2);
    iw3.MoveWindow(x3, y3);
    iw4.MoveWindow(x4, y4);

    ros::spin();

}
