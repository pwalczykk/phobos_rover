#include "ROS_UART_Rx.hpp"

int main(int argc, char** argv){

    ros::init(argc, argv, "uart_reciver");
    ros::NodeHandle nh;

    ROS_UART_Rx rx("/dev/ttyAMA0", "/rover/uart/rx", &nh);

    ros::Rate loop_rate(100);

    while(ros::ok()){
        rx.CheckReciveBuffer();
        loop_rate.sleep();
    }
    return 0;
}
