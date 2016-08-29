#include "CameraHandler.hpp"
#include "ImagePublisher.hpp"
#include <ros/ros.h>

int main(int argc, char** argv){

    // TODO : Why argc -4/-3 instead of -2/-1 ??? WTF, but works!
    const std::string stream_addres = argv[argc-4];
    const std::string publisher_topic = argv[argc-3];

    ROS_WARN_STREAM(stream_addres);
    ROS_WARN_STREAM(publisher_topic);

    ros::init(argc, argv, "ip_camera_driver");
    ros::NodeHandle nh;

    ImagePublisher cam1(stream_addres, publisher_topic);

    // cam1.CameraHandler::Set(320, 240, 30);

    ros::Rate loop_rate(10);
    while(ros::ok()){
        cam1.Init();
        cam1.PublishImage();
        loop_rate.sleep();
    }

    cam1.CameraHandler::Release();

    return 0;
}
