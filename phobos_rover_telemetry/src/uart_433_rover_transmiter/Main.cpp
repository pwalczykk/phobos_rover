#include <ros/ros.h>

#include "../../../../../phobos_shared/src/phobos_shared/include/UART_Tx.hpp"

#include "SubOdom.hpp"
#include "SubJointsState.hpp"
#include "SubError.hpp"

int main(int argc, char** argv){

    ros::init(argc, argv, "uart_433_rover_transmiter");
    ros::NodeHandle nh;

    UART_Tx <FrameTelemetry>tx("/dev/ttyAMA0", TELEMETRY_DATA_NUM);

    SubOdom odom("/rover/localization/odom_ekf", &nh);
    SubJointsState joints_state("/rover/encoders/joints_state", &nh);
    SubError error_control("/rover/security/error_code", &nh);

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

        tx.WORD.wheel_vel_fl = joints_state.msg.wheel_vel_fl;
        tx.WORD.wheel_vel_fr = joints_state.msg.wheel_vel_fr;
        tx.WORD.wheel_vel_ml = joints_state.msg.wheel_vel_ml;
        tx.WORD.wheel_vel_mr = joints_state.msg.wheel_vel_mr;
        tx.WORD.wheel_vel_bl = joints_state.msg.wheel_vel_bl;
        tx.WORD.wheel_vel_br = joints_state.msg.wheel_vel_br;
        tx.WORD.link_pose_0 = joints_state.msg.link_pose_0;
        tx.WORD.link_pose_1 = joints_state.msg.link_pose_1;
        tx.WORD.link_pose_2 = joints_state.msg.link_pose_2;
        tx.WORD.link_pose_3 = joints_state.msg.link_pose_3;
        tx.WORD.link_pose_4 = joints_state.msg.link_pose_4;
        tx.WORD.grip_pose = joints_state.msg.grip_pose;
        tx.WORD.rocker_pose_l = joints_state.msg.rocker_pose_l;
        tx.WORD.rocker_pose_r = joints_state.msg.rocker_pose_r;
        tx.WORD.bogie_pose_l = joints_state.msg.bogie_pose_l;
        tx.WORD.bogie_pose_r = joints_state.msg.bogie_pose_r;

        tx.WORD.error_code = error_control.msg.data;
        tx.WORD.control_sum = tx.ControlSum();

        tx.Transmit();
        loop_rate.sleep();
        // ROS_INFO("TX: %d %d %d %d %d %d %d %d", *(word+0), *(word+1), *(word+2), *(word+3), *(word+4), *(word+5), *(word+6), *(word+7));
    }
    return 0;
}
