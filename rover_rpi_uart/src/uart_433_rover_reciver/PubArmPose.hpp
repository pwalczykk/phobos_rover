#ifndef PubArmPose_HPP_
#define PubArmPose_HPP_

#include <ros/ros.h>
#include <rover_rpi_uart/ArmPose5.h>

class PubArmPose{
    ros::NodeHandle *nh;
    ros::Publisher pub;

    rover_rpi_uart::ArmPose5 msg;
public:
    PubArmPose(std::string topic, ros::NodeHandle *nh){
        this->nh = nh;
        this->pub = nh->advertise<rover_rpi_uart::ArmPose5>(topic, 100);
    }
    ~PubArmPose(){}

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
