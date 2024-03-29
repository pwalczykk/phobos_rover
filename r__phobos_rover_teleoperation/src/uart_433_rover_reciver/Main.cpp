#include <ros/ros.h>

#include "../../../../../phobos_shared/src/phobos_shared/include/UART_Rx.hpp"

#include "PubWheelsVel.hpp"
#include "PubArmVel.hpp"

int main(int argc, char** argv){
    int BASIC_RATE;
    int SYNCHRO_RATE;

    ros::init(argc, argv, "uart_433_rover_reciver");
    ros::NodeHandle nh;

    nh.param("basic_rate", BASIC_RATE, 5);
    nh.param("synchro_rate", SYNCHRO_RATE, 21);

    ROS_INFO("Reciver start");

    UART_Rx <FrameTeleoperation>rx("/dev/ttyAMA0", TELEOPERATION_DATA_NUM, TELEOPERATION_BUFFOR_SIZE);

    PubWheelsVel wheels_vel("/rover/control/wheels_vel", &nh);
    PubArmVel arm_vel("/rover/control/arm_vel", &nh);


    int UART_SYNCHRO = 0;
    int ERROR_COUNTER = 0;
    int MAX_ERROR_NUM = SYNCHRO_RATE;

    ros::Rate loop_rate(BASIC_RATE);
    ros::Rate synchro_rate(SYNCHRO_RATE);

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

                UART_SYNCHRO = 1;
                ERROR_COUNTER = 0;
            }else{
                ERROR_COUNTER++;
            }
        }else{
            ERROR_COUNTER++;
        }

        if(ERROR_COUNTER > MAX_ERROR_NUM){
            exit(-1);
        }

        if(UART_SYNCHRO == 1){
            loop_rate.sleep();
        }else{
            synchro_rate.sleep();
        }
    }
    return 0;
}
