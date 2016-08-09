#include <ros/ros.h>

#include "../../../../../phobos_shared/src/phobos_shared/include/UART_Rx.hpp"

#include "PubWheelsVel.hpp"
#include "PubArmVel.hpp"

int main(int argc, char** argv){

    ros::init(argc, argv, "uart_433_rover_reciver");
    ros::NodeHandle nh;

    ROS_INFO("Reciver start");

    UART_Rx <FrameTeleoperation>rx("/dev/ttyAMA0", TELEOPERATION_DATA_NUM, TELEOPERATION_BUFFOR_SIZE);

    PubWheelsVel wheels_vel("/rover/control/wheels_vel", &nh);
    PubArmVel arm_vel("/rover/control/arm_vel", &nh);

    ros::Rate loop_rate(5);

    while(ros::ok()){
        // if(rx.ReadBuffer()){
        if(rx.ReadBufferAsChar64()){
            if(rx.CheckControlSum()){
                int wheels_left = rx.WORD.wheels_left;
                int wheels_right = rx.WORD.wheels_right;

                int link_0  = rx.WORD.link_0;
                int link_1  = rx.WORD.link_1;
                int link_2  = rx.WORD.link_2;
                int link_3  = rx.WORD.link_3;
                int link_4  = rx.WORD.link_4;
                int grip_force = rx.WORD.grip_force;

                wheels_vel.Publish(wheels_left, wheels_right);
                arm_vel.Publish(link_0, link_1, link_2, link_3, link_4, grip_force);
            }else{
                ROS_WARN("Wrong control sum");
            }
            // ROS_INFO("RX: %d %d %d %d %d %d %d %d %d", *(rx.WORD.begin+0), *(rx.WORD.begin+1), *(rx.WORD.begin+2), *(rx.WORD.begin+3), *(rx.WORD.begin+4), *(rx.WORD.begin+5), *(rx.WORD.begin+6), *(rx.WORD.begin+7), *(rx.WORD.begin+8));
        }else{
            ROS_WARN("Cant read buffer");
        }
        loop_rate.sleep();
    }
    return 0;
}
