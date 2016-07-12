#ifndef ROS_UART_RX_HPP_
#define ROS_UART_RX_HPP_

#include <ros/ros.h>
#include <std_msgs/String.h>

#include <string.h>

#include "UART_Rx.hpp"

class ROS_UART_Rx : public UART_Rx{
    ros::NodeHandle *nh;
    ros::Publisher pub;

    std_msgs::String msg;
public:
    ROS_UART_Rx(std::string device_addres, std::string topic, ros::NodeHandle* nh) : UART_Rx((const char*) device_addres.c_str()){
        this->nh = nh;
        pub = nh->advertise<std_msgs::String>(topic, 100);
    }

    ~ROS_UART_Rx(){}

    void CheckReciveBuffer(){
        if(UART_Rx::ReadBuffer()){
            msg.data = std::string(UART_Rx::rx_buffer);
            pub.publish(msg);
        }
    }
};

#endif
