#include <ros/ros.h>

#include <phobos_shared/EncodersArm.h>
#include <phobos_shared/EncodersWheels.h>
#include <phobos_shared/EncodersRockerBogie.h>

#include "../../../../../phobos_shared/src/phobos_shared/include/UART_Tx.hpp"

#include "SubOdom.hpp"
#include "SubEncoders.hpp"
#include "SubError.hpp"

int main(int argc, char** argv){

    ros::init(argc, argv, "uart_433_rover_transmiter");
    ros::NodeHandle nh;

    UART_Tx <FrameTelemetry>tx("/dev/ttyAMA0", TELEMETRY_DATA_NUM);

    SubOdom odom("/rover/localization/odom_ekf", &nh);
    SubError error_control("/rover/security/error_code", &nh);

    SubEncoders <phobos_shared::EncodersArm> arm_encoders ("/rover/encoders/arm", &nh);
    SubEncoders <phobos_shared::EncodersWheels> wheels_encoders ("/rover/encoders/wheels", &nh);
    SubEncoders <phobos_shared::EncodersRockerBogie> rocker_bogie_encoders ("/rover/encoders/rocker_bogie", &nh);

    ros::Rate loop_rate(10);

    while(ros::ok()){
        ros::spinOnce();
        tx.WORD.position_x = odom.msg.position.x;
        tx.WORD.position_y = odom.msg.position.y;
        tx.WORD.position_z = odom.msg.position.z;
        tx.WORD.orientation_x = odom.msg.orientation.x;
        tx.WORD.orientation_y = odom.msg.orientation.y;
        tx.WORD.orientation_z = odom.msg.orientation.z;
        tx.WORD.orientation_w = odom.msg.orientation.w;

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

        tx.Transmit();
        loop_rate.sleep();
        // ROS_INFO("TX: %d %d %d %d %d %d %d %d", *(word+0), *(word+1), *(word+2), *(word+3), *(word+4), *(word+5), *(word+6), *(word+7));
    }
    return 0;
}
