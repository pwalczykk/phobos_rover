#include <ros/ros.h>
#include "UART_Rx.hpp"
#include "PubWheelsVel.hpp"
#include "PubArmVel.hpp"

int main(int argc, char** argv){

    ros::init(argc, argv, "uart_433_rover_reciver");
    ros::NodeHandle nh;

    ROS_INFO("Reciver start");

    UART_Rx rx("/dev/ttyAMA0");

    PubWheelsVel wheels_vel("/rover/control/wheels_vel", &nh);
    PubArmVel arm_vel("/rover/control/arm_vel", &nh);

    ros::Rate loop_rate(10);

    while(ros::ok()){
        if(rx.ReadBuffer()){
            if(rx.CheckControlSum()){
                int wheels_left = rx.rx_buffer[0];
                int wheels_right = rx.rx_buffer[1];

                int link_0  = rx.rx_buffer[2];
                int link_1  = rx.rx_buffer[3];
                int link_2  = rx.rx_buffer[4];
                int link_3  = rx.rx_buffer[5];
                int link_4  = rx.rx_buffer[6];

                wheels_vel.Publish(wheels_left, wheels_right);
                arm_vel.Publish(link_0, link_1, link_2, link_3, link_4);
            }else{
                ROS_WARN("Wrong control sum");
            }
            ROS_INFO("RX: %d %d %d %d %d %d %d %d", *(rx.rx_buffer+0), *(rx.rx_buffer+1), *(rx.rx_buffer+2), *(rx.rx_buffer+3), *(rx.rx_buffer+4), *(rx.rx_buffer+5), *(rx.rx_buffer+6), *(rx.rx_buffer+7));
        }else{
            ROS_WARN("Cant read buffer");
        }
        loop_rate.sleep();
    }
    return 0;
}
