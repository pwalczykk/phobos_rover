#ifndef PubWheelsVel_HPP_
#define PubWheelsVel_HPP_

#include <ros/ros.h>
#include <rover_rpi_uart/WheelsVel2.h>

class PubWheelsVel{
    ros::NodeHandle *nh;
    ros::Publisher pub;

    rover_rpi_uart::WheelsVel2 msg;
public:
    PubWheelsVel(std::string topic, ros::NodeHandle *nh){
        this->nh = nh;
        this->pub = nh->advertise<rover_rpi_uart::WheelsVel2>(topic, 100);
    }
    ~PubWheelsVel(){}

    void Publish(int wheels_left, int wheels_right){
        this->msg.wheels_left = wheels_left;
        this->msg.wheels_right = wheels_right;
        this->pub.publish(msg);
    }
};

#endif
