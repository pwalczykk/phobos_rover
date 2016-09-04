#include <ros/ros.h>
#include "PubCamInfo.hpp"

int main(int argc, char** argv){
    ros::init(argc, argv, "cam_info_publisher");
    ros::NodeHandle nh("~");

    std::string camera_name;
    nh.getParam("camera_name", camera_name);

    PubCamInfo caminfo(camera_name, &nh);

    ros::Rate loop_rate(10);

    while(ros::ok()){
        caminfo.msg.header.seq++;
        // TODO: wypełnienie messageu

        caminfo.Publish();
        loop_rate.sleep();
    }
}
