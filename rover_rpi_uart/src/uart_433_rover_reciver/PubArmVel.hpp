#ifndef PubArmVel_HPP_
#define PubArmVel_HPP_

#include <ros/ros.h>
#include <rover_rpi_uart/ArmPose5.h>

class PubArmVel{
    ros::NodeHandle *nh;
    ros::Publisher pub;

    rover_rpi_uart::ArmPose5 msg;
public:
    PubArmVel(std::string topic, ros::NodeHandle *nh){
        this->nh = nh;
        this->pub = nh->advertise<rover_rpi_uart::ArmPose5>(topic, 100);
    }
    ~PubArmVel(){}

    void Publish(int link0, int link1, int link2, int link3, int link4){
        this->msg.link0 = link0;
        this->msg.link1 = link1;
        this->msg.link2 = link2;
        this->msg.link3 = link3;
        this->msg.link4 = link4;
        this->pub.publish(msg);
    }
};

#endif
