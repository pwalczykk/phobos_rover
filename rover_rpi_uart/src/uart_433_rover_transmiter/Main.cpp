#include "UART_Tx.hpp"
#include "SubPoseOrient.hpp"
#include <stdlib.h>


int main(int argc, char** argv){

    ros::init(argc, argv, "uart_433_rover_transmiter");
    ros::NodeHandle nh;

    UART_Tx tx("/dev/ttyAMA0");

    SubPoseOrient pose_orient("/rover/pose", &nh);

    ros::Rate loop_rate(1);

    int32_t* word = (int32_t*) malloc((BUFF_SIZE) * sizeof(int32_t));

    while(ros::ok()){
        ROS_WARN("1");
        ros::spinOnce();
        ROS_WARN("2");
        *(word+0) = pose_orient.msg.position.x;
        ROS_WARN("3");
        *(word+1) = pose_orient.msg.position.y;
        ROS_WARN("4");
        *(word+2) = pose_orient.msg.position.z;
        ROS_WARN("5");
        *(word+3) = pose_orient.msg.orientation.x;
        ROS_WARN("6");
        *(word+4) = pose_orient.msg.orientation.y;
        ROS_WARN("7");
        *(word+5) = pose_orient.msg.orientation.z;
        ROS_WARN("8");
        *(word+6) = pose_orient.msg.orientation.w;
        ROS_WARN("9");
        *(word+7) = tx.ControlSum(word);
        ROS_WARN("10");
        tx.Transmit(word);
        ROS_WARN("11");
        loop_rate.sleep();
        ROS_WARN("12");
        ROS_INFO("TX: %d %d %d %d %d %d %d %d", *(word+0), *(word+1), *(word+2), *(word+3), *(word+4), *(word+5), *(word+6), *(word+7));
    }

    // free(word);
    return 0;
}
