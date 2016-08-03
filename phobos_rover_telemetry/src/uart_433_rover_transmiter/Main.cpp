#include "UART_Tx.hpp"
#include "SubPoseOrient.hpp"
#include <stdlib.h>


int main(int argc, char** argv){

    ros::init(argc, argv, "uart_433_rover_transmiter");
    ros::NodeHandle nh;

    UART_Tx tx("/dev/ttyAMA0");

    SubPoseOrient pose_orient("/rover/pose", &nh);

    ros::Rate loop_rate(10);

    int32_t* word = (int32_t*) malloc((BUFF_SIZE) * sizeof(int32_t));

    while(ros::ok()){
        ros::spinOnce();
        *(word+0) = pose_orient.msg.position.x;
        *(word+1) = pose_orient.msg.position.y;
        *(word+2) = pose_orient.msg.position.z;
        *(word+3) = pose_orient.msg.orientation.x;
        *(word+4) = pose_orient.msg.orientation.y;
        *(word+5) = pose_orient.msg.orientation.z;
        *(word+6) = pose_orient.msg.orientation.w;
        *(word+7) = tx.ControlSum(word);
        tx.Transmit(word);
        loop_rate.sleep();
        ROS_INFO("TX: %d %d %d %d %d %d %d %d", *(word+0), *(word+1), *(word+2), *(word+3), *(word+4), *(word+5), *(word+6), *(word+7));
    }

    free(word);
    return 0;
}
