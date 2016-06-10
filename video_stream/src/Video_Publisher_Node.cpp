#include <ros/ros.h>
#include "../include/Video_Publisher.hpp"
#include "sensor_msgs/CameraInfo.h"

int cam_num = 0;

int main(int argc, char** argv)
{

    if(1){
        cam_num = atoi(argv[argc-1]);
        ROS_WARN("%d", cam_num);
    }

    ros::init(argc, argv, "video_stream");
    Video_Publisher vp(cam_num);
    vp.Camera_init();          // ROS_INFO("debug0");

    ros::NodeHandle nh;

    while(ros::ok())
    {
        vp.Capture_frame();    // ROS_INFO("debug1");
        vp.Edit_frame();       // ROS_INFO("debug2");
        vp.Publish_frame();    // ROS_INFO("debug3");
        vp.Rate_sleep();       // ROS_INFO("debug4");
    }
}
