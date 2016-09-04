#include <ros/ros.h>

#include <phobos_shared/EncodersArm.h>
#include <phobos_shared/EncodersWheels.h>
#include <phobos_shared/EncodersRockerBogie.h>

#include "../../../../../phobos_shared/src/phobos_shared/include/UART_Shared.hpp"
#include "../../../../../phobos_shared/src/phobos_shared/include/Conversions.hpp"

#include "PubCtrl.hpp"
#include "PubWheelsVel.hpp"
#include "PubArmVel.hpp"

#include "SubOdom.hpp"
#include "SubEncoders.hpp"
#include "SubError.hpp"

int main(int argc, char** argv){
    int BASIC_RATE;
    int SYNCHRO_RATE;

    ros::init(argc, argv, "uart_433_rover_transceiver");
    ros::NodeHandle nh;

    nh.param("basic_rate", BASIC_RATE, 5);
    nh.param("synchro_rate", SYNCHRO_RATE, 5);

    // UART_Tx <FrameTelemetry>tx("/dev/ttyAMA0", TELEMETRY_DATA_NUM, TELEMETRY_BUFFOR_SIZE);
    // UART_Rx <FrameTeleoperation>rx("/dev/ttyAMA0", TELEOPERATION_DATA_NUM, TELEOPERATION_BUFFOR_SIZE);

    UART_Rx RX("/dev/ttyAMA0");
    UART_Rx_Decoder <FrameTeleoperationCtrl>    rx_ctrl     (&RX, TO_CTRL_DATA_NUM,     TO_CTRL_DATA_SIZE,      TO_CTRL_BUFFOR_SIZE);
    UART_Rx_Decoder <FrameTeleoperationWheels>  rx_wheels   (&RX, TO_WHEELS_DATA_NUM,   TO_WHEELS_DATA_SIZE,    TO_WHEELS_BUFFOR_SIZE);
    UART_Rx_Decoder <FrameTeleoperationArm>     rx_arm      (&RX, TO_ARM_DATA_NUM,      TO_ARM_DATA_SIZE,       TO_ARM_BUFFOR_SIZE);

    UART_Tx TX("/dev/ttyAMA0");
    UART_Tx_Encoder <FrameTelemetryPose>    tx_pose    (&TX, TM_POSE_DATA_NUM,      TM_POSE_DATA_SIZE,      TM_POSE_BUFFOR_SIZE);
    UART_Tx_Encoder <FrameTelemetryOrient>  tx_orient  (&TX, TM_ORIENT_DATA_NUM,    TM_ORIENT_DATA_SIZE,    TM_ORIENT_BUFFOR_SIZE);
    UART_Tx_Encoder <FrameTelemetryWheels>  tx_wheels  (&TX, TM_WHEELS_DATA_NUM,    TM_WHEELS_DATA_SIZE,    TM_WHEELS_BUFFOR_SIZE);
    UART_Tx_Encoder <FrameTelemetryArm>     tx_arm     (&TX, TM_ARM_DATA_NUM,       TM_ARM_DATA_SIZE,       TM_ARM_BUFFOR_SIZE);
    UART_Tx_Encoder <FrameTelemetrySusp>    tx_susp    (&TX, TM_SUSP_DATA_NUM,      TM_SUSP_DATA_SIZE,      TM_SUSP_BUFFOR_SIZE);

    PubCtrl pub_ctrl("/rover/security/ctrl", &nh);
    PubWheelsVel pub_wheels("/rover/control/wheels_vel", &nh);
    PubArmVel pub_arm("/rover/control/arm_pose", &nh);

    SubOdom sub_odom("/rover/localization/odom_ekf", &nh);
    SubError sub_error("/rover/security/error_code", &nh);
    SubEncoders <phobos_shared::EncodersArm> sub_arm_encoders ("/rover/encoders/arm_absolute", &nh);
    SubEncoders <phobos_shared::EncodersWheels> sub_wheels_encoders ("/rover/encoders/wheels_relative", &nh);
    SubEncoders <phobos_shared::EncodersRockerBogie> sub_rocker_bogie_encoders ("/rover/encoders/rocker_bogie_absolute", &nh);

    int UART_SYNCHRO = 0;
    int ERROR_COUNTER = 0;
    int MAX_ERROR_NUM = 5;
    int RECIVED_FIRST_DATA = 0;

    int tx_counter = 0;

    ros::Rate loop_rate(BASIC_RATE);
    ros::Rate synchro_rate(SYNCHRO_RATE);

    while(ros::ok()){
        //RECIVER
        if(RX.ReadBuffor()){
            switch(RX.DecodeMessageType()){
                case FRAME_TO_CTRL: {
                    if(rx_ctrl.CheckControlSum()){
                        rx_ctrl.DecodeBuffor();

                        pub_ctrl.msg.data = rx_ctrl.FRAME.header.ctrl;
                        pub_ctrl.Publish();

                        RECIVED_FIRST_DATA = 1; UART_SYNCHRO = 1; ERROR_COUNTER = 0;
                    }
                }
                case FRAME_TO_WHEELS: {
                    if(rx_wheels.CheckControlSum()){
                        rx_wheels.DecodeBuffor();

                        pub_ctrl.msg.data = rx_wheels.FRAME.header.ctrl;
                        pub_ctrl.Publish();

                        pub_wheels.msg.wheels_left = rx_wheels.FRAME.wheels_left;
                        pub_wheels.msg.wheels_right = rx_wheels.FRAME.wheels_right;
                        pub_wheels.Publish();

                        RECIVED_FIRST_DATA = 1; UART_SYNCHRO = 1; ERROR_COUNTER = 0;
                    }
                }
                case FRAME_TO_ARM: {
                    if(rx_arm.CheckControlSum()){
                        rx_arm.DecodeBuffor();

                        pub_ctrl.msg.data = rx_arm.FRAME.header.ctrl;
                        pub_ctrl.Publish();

                        pub_arm.msg.link_0 = rx_arm.FRAME.link_0;
                        pub_arm.msg.link_1 = rx_arm.FRAME.link_1;
                        pub_arm.msg.link_2 = rx_arm.FRAME.link_2;
                        pub_arm.msg.link_3 = rx_arm.FRAME.link_3;
                        pub_arm.msg.link_4 = rx_arm.FRAME.link_4;
                        pub_arm.msg.gripper = rx_arm.FRAME.grip;
                        pub_arm.Publish();

                        RECIVED_FIRST_DATA = 1; UART_SYNCHRO = 1; ERROR_COUNTER = 0;
                    }
                }
                default : {
                    ROS_WARN("RX frame not recognized! Type: %c (ASCII: %d)", RX.DecodeMessageType(), (int)RX.DecodeMessageType());
                    ROS_WARN("Availablfe frames: %c %c %c", FRAME_TO_CTRL, FRAME_TO_WHEELS, FRAME_TO_ARM);
                    ERROR_COUNTER++;
                }
            }
        }else{
            ERROR_COUNTER++;
        }

        if(ERROR_COUNTER > MAX_ERROR_NUM){
            ROS_ERROR("%d errors in a row - exiting!!", ERROR_COUNTER);
            exit(-1);
        }


        if(RECIVED_FIRST_DATA == 0){
            break;
        }

        // TRANSMITER
        ros::spinOnce();
        switch (tx_counter){
            case 0: {   // POSITION
                tx_pose.FRAME.header.type = FRAME_TM_POSE;
                tx_pose.FRAME.header.ctrl = sub_error.msg.data;

                tx_pose.FRAME.position_x = Pose_Float2Int(sub_odom.msg.pose.pose.position.x);
                tx_pose.FRAME.position_y = Pose_Float2Int(sub_odom.msg.pose.pose.position.y);
                tx_pose.FRAME.position_z = Pose_Float2Int(sub_odom.msg.pose.pose.position.z);

                tx_pose.EncodeBuffor();

                TX.Transmit(TM_POSE_BUFFOR_SIZE);
            }

            case 1: {   // ORIENTATION
                tx_orient.FRAME.header.type = FRAME_TM_ORIENT;
                tx_orient.FRAME.header.ctrl = sub_error.msg.data;

                tx_orient.FRAME.orientation_x = Orient_Float2Int(sub_odom.msg.pose.pose.orientation.x);
                tx_orient.FRAME.orientation_y = Orient_Float2Int(sub_odom.msg.pose.pose.orientation.y);
                tx_orient.FRAME.orientation_z = Orient_Float2Int(sub_odom.msg.pose.pose.orientation.z);
                tx_orient.FRAME.orientation_w = Orient_Float2Int(sub_odom.msg.pose.pose.orientation.w);

                tx_orient.EncodeBuffor();

                TX.Transmit(TM_ORIENT_BUFFOR_SIZE);
            }

            case 2: {   // WHEELS ENCODERS

                tx_wheels.FRAME.header.type = FRAME_TM_WHEELS;
                tx_wheels.FRAME.header.ctrl = sub_error.msg.data;

                tx_wheels.FRAME.wheel_vel_fl = sub_wheels_encoders.msg.wheel_vel_fl;
                tx_wheels.FRAME.wheel_vel_fr = sub_wheels_encoders.msg.wheel_vel_fr;
                tx_wheels.FRAME.wheel_vel_ml = sub_wheels_encoders.msg.wheel_vel_ml;
                tx_wheels.FRAME.wheel_vel_mr = sub_wheels_encoders.msg.wheel_vel_mr;
                tx_wheels.FRAME.wheel_vel_bl = sub_wheels_encoders.msg.wheel_vel_bl;
                tx_wheels.FRAME.wheel_vel_br = sub_wheels_encoders.msg.wheel_vel_br;

                tx_wheels.EncodeBuffor();

                TX.Transmit(TM_WHEELS_BUFFOR_SIZE);
            }

            case 3: {   // ARM ENCODERS
                tx_arm.FRAME.header.type = FRAME_TM_ARM;
                tx_arm.FRAME.header.ctrl = sub_error.msg.data;

                tx_arm.FRAME.link_pose_0 = sub_arm_encoders.msg.link_pose_0;
                tx_arm.FRAME.link_pose_1 = sub_arm_encoders.msg.link_pose_1;
                tx_arm.FRAME.link_pose_2 = sub_arm_encoders.msg.link_pose_2;
                tx_arm.FRAME.link_pose_3 = sub_arm_encoders.msg.link_pose_3;
                tx_arm.FRAME.link_pose_4 = sub_arm_encoders.msg.link_pose_4;
                tx_arm.FRAME.grip_pose = sub_arm_encoders.msg.grip_pose;

                tx_arm.EncodeBuffor();

                TX.Transmit(TM_ARM_BUFFOR_SIZE);
            }

            case 4: {   // SUSPENSION ENCODERS
                tx_susp.FRAME.header.type = FRAME_TM_SUSP;
                tx_susp.FRAME.header.ctrl = sub_error.msg.data;

                tx_susp.FRAME.rocker_pose_l = sub_rocker_bogie_encoders.msg.rocker_pose_l;
                tx_susp.FRAME.rocker_pose_r = sub_rocker_bogie_encoders.msg.rocker_pose_r;
                tx_susp.FRAME.bogie_pose_l = sub_rocker_bogie_encoders.msg.bogie_pose_l;
                tx_susp.FRAME.bogie_pose_r = sub_rocker_bogie_encoders.msg.bogie_pose_r;

                tx_susp.EncodeBuffor();

                TX.Transmit(TM_SUSP_BUFFOR_SIZE);
            }
        }

        if(tx_counter == 4){
            tx_counter = 0;
        }else{
            tx_counter++;
        }

        if(UART_SYNCHRO == 1){
            loop_rate.sleep();
        }else{
            synchro_rate.sleep();
        }
    }


    return 0;
}
