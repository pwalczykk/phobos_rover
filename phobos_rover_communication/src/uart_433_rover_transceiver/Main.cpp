#include <ros/ros.h>

#include <phobos_shared/EncodersArm.h>
#include <phobos_shared/EncodersWheels.h>
#include <phobos_shared/EncodersRockerBogie.h>

#include "../../../../../phobos_shared/src/phobos_shared/include/UART_Tx.hpp"
#include "../../../../../phobos_shared/src/phobos_shared/include/UART_Rx.hpp"

#include "SubOdom.hpp"
#include "SubEncoders.hpp"
#include "SubError.hpp"
#include "PubWheelsVel.hpp"
#include "PubArmVel.hpp"

int main(int argc, char** argv){
    int BASIC_RATE;
    int SYNCHRO_RATE;

    ros::init(argc, argv, "uart_433_rover_transceiver");
    ros::NodeHandle nh;

    nh.param("basic_rate", BASIC_RATE, 5);
    // nh.param("synchro_rate", SYNCHRO_RATE, 21);

    UART_Tx <FrameTelemetry>tx("/dev/ttyAMA0", TELEMETRY_DATA_NUM, TELEMETRY_BUFFOR_SIZE);
    UART_Rx <FrameTeleoperation>rx("/dev/ttyAMA0", TELEOPERATION_DATA_NUM, TELEOPERATION_BUFFOR_SIZE);

    SubOdom odom("/rover/localization/odom_ekf", &nh);
    SubError error_control("/rover/security/error_code", &nh);
    SubEncoders <phobos_shared::EncodersArm> arm_encoders ("/rover/encoders/arm_absolute", &nh);
    SubEncoders <phobos_shared::EncodersWheels> wheels_encoders ("/rover/encoders/wheels_relative", &nh);
    SubEncoders <phobos_shared::EncodersRockerBogie> rocker_bogie_encoders ("/rover/encoders/rocker_bogie_absolute", &nh);

    PubWheelsVel wheels_vel("/rover/control/wheels_vel", &nh);
    PubArmVel arm_vel("/rover/control/arm_vel", &nh);

    int UART_SYNCHRO = 0;
    int ERROR_COUNTER = 0;
    int MAX_ERROR_NUM = SYNCHRO_RATE;
    int RECIVED_FIRST_DATA = 0;


    ros::Rate loop_rate(BASIC_RATE);
    // ros::Rate synchro_rate(SYNCHRO_RATE);

    while(ros::ok()){

        // RECIVER
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

                RECIVED_FIRST_DATA = 1;
                UART_SYNCHRO = 1;
                ERROR_COUNTER = 0;
            }else{
                ERROR_COUNTER++;
            }
        }else{
            ERROR_COUNTER++;
        }

        if(ERROR_COUNTER > MAX_ERROR_NUM){
            // exit(-1);
        }

        if(RECIVED_FIRST_DATA == 0){
            break;
        }

        // TRANSMITER
        ros::spinOnce();
        tx.WORD.position_x = odom.msg.pose.pose.position.x;
        tx.WORD.position_y = odom.msg.pose.pose.position.y;
        tx.WORD.position_z = odom.msg.pose.pose.position.z;
        tx.WORD.orientation_x = odom.msg.pose.pose.orientation.x;
        tx.WORD.orientation_y = odom.msg.pose.pose.orientation.y;
        tx.WORD.orientation_z = odom.msg.pose.pose.orientation.z;
        tx.WORD.orientation_w = odom.msg.pose.pose.orientation.w;

        tx.WORD.wheel_vel_fl = wheels_encoders.msg.wheel_vel_fl;
        tx.WORD.wheel_vel_fr = wheels_encoders.msg.wheel_vel_fr;
        tx.WORD.wheel_vel_ml = wheels_encoders.msg.wheel_vel_ml;
        tx.WORD.wheel_vel_mr = wheels_encoders.msg.wheel_vel_mr;
        tx.WORD.wheel_vel_bl = wheels_encoders.msg.wheel_vel_bl;
        tx.WORD.wheel_vel_br = wheels_encoders.msg.wheel_vel_br;

        tx.WORD.link_pose_0 = arm_encoders.msg.link_pose_0;
        tx.WORD.link_pose_1 = arm_encoders.msg.link_pose_1;
        tx.WORD.link_pose_2 = arm_encoders.msg.link_pose_2;
        tx.WORD.link_pose_3 = arm_encoders.msg.link_pose_3;
        tx.WORD.link_pose_4 = arm_encoders.msg.link_pose_4;
        tx.WORD.grip_pose = arm_encoders.msg.grip_pose;

        tx.WORD.rocker_pose_l = rocker_bogie_encoders.msg.rocker_pose_l;
        tx.WORD.rocker_pose_r = rocker_bogie_encoders.msg.rocker_pose_r;
        tx.WORD.bogie_pose_l = rocker_bogie_encoders.msg.bogie_pose_l;
        tx.WORD.bogie_pose_r = rocker_bogie_encoders.msg.bogie_pose_r;

        tx.WORD.error_code = error_control.msg.data;
        tx.WORD.control_sum = tx.ControlSum();

        tx.TransmitAsChar64();


        // if(UART_SYNCHRO == 1){
            loop_rate.sleep();
        // }else{
        //     synchro_rate.sleep();
        // }

    }
    return 0;
}
