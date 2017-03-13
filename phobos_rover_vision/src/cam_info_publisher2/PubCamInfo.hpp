#ifndef PUB_CAM_INFO_HPP_
#define PUB_CAM_INFO_HPP_

#include <ros/ros.h>
#include <sensor_msgs/CameraInfo.h>

class PubCamInfo{
    ros::NodeHandle *nh;
    ros::Publisher pub;

public:
    sensor_msgs::CameraInfo msg;

public:
    PubCamInfo(std::string camera_name, int width, int height, ros::NodeHandle *nh){
        this->nh = nh;
        this->pub = nh->advertise<sensor_msgs::CameraInfo>("/rover/" + camera_name + "/camera_info", 100);

        this->msg.header.seq = 0;
        this->msg.header.frame_id = camera_name;

        this->msg.height = height;
        this->msg.width = width;

        this->msg.roi.x_offset = 0;
        this->msg.roi.y_offset = 0;
        this->msg.roi.height = 0;
        this->msg.roi.width = 0;
        this->msg.roi.do_rectify = false;

    }

    void Publish(){
        this->msg.header.seq++;
        pub.publish(this->msg);
    }
};

#endif
