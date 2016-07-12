#ifndef ROS_UART_TX_HPP_
#define ROS_UART_TX_HPP_

#include <ros/ros.h>
#include <std_msgs/String.h>

#include <string.h>

#include "UART_Tx.hpp"

class ROS_UART_Tx : public UART_Tx{
    ros::NodeHandle *nh;
    ros::Subscriber sub;

public:
    ROS_UART_Tx(std::string device_addres, std::string topic, ros::NodeHandle* nh) : UART_Tx((const char*) device_addres.c_str()){
        this->nh = nh;
        sub = nh->subscribe(topic, 100, &ROS_UART_Tx::MsgRecived, this);
    }

    ~ROS_UART_Tx(){}

    void MsgRecived(const std_msgs::String msg){
        UART_Tx::Transmit((char*)msg.data.c_str());
    }
};

#endif
