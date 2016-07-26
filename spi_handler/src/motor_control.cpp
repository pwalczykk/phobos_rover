#include <string>
#include <iostream>
#include <sstream>
#include "../include/SPI_Master.hpp"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <rover_rpi_uart/WheelsVel2.h>

using namespace std;

SPI_Master spi_master;

string getEncoders() {
    uint8_t txBuffer[21] = {0x73, 0x04, 0x4D, 0x67, 0x00, 0x00, 0x00, 0x00, 0x2E, 0x65};
    uint8_t rxBuffer[21] = {0,};
    spi_master.WriteData(txBuffer,rxBuffer, 21);
    stringstream ss;
    ss << (uint)rxBuffer[10] << " " << (uint)rxBuffer[11] << " " << (uint)rxBuffer[12] << " " << (uint)rxBuffer[13] << " " << (uint)rxBuffer[14] << " " << (uint)rxBuffer[15] << " " << (uint)rxBuffer[16] << " " << (uint)rxBuffer[17] << " " << (uint)rxBuffer[18] << " " << (uint)rxBuffer[19] << " " << (uint)rxBuffer[20] << endl;
    return ss.str();
}

void setMotors(const rover_rpi_uart::WheelsVel2::ConstPtr& msg) {
    ROS_INFO("I heard: [%d/%d]", msg->wheels_left, msg->wheels_right);
    uint16_t wleft = msg->wheels_left;
    uint16_t wright = msg->wheels_right;
    uint8_t L = (wleft >> 8) && 0xFF;
    uint8_t l = wleft && 0xFF;
    uint8_t P = 0;
    uint8_t p = 0;
    uint8_t CRC = (0x04 ^ 0x4D ^ 0x73 ^ L ^ l ^ P ^ p);
    uint8_t txBuffer[16] = {0x73, 0x04, 0x4D, 0x73, L, l, P, p, CRC, 0x65};
    uint8_t rxBuffer[16] = {0,};
    spi_master.WriteData(txBuffer,rxBuffer, 16);
}

int main(int argc, char** argv)
{
    spi_master.OpenBus();
    ros::init(argc,argv,"motor_control");
    ros::NodeHandle n;
    ros::Rate loop_rate(10);

    ros::Subscriber pwms = n.subscribe("motors", 1000, setMotors);
    ros::Publisher encoders = n.advertise<std_msgs::String>("encoders", 1000);

    while (ros::ok())
    {
        std_msgs::String msg;
        msg.data = getEncoders();
        encoders.publish(msg);

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

