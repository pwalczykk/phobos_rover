#include "ROS_UART_Tx.hpp"

int main(int argc, char** argv){

    ros::init(argc, argv, "uart_transmiter");
    ros::NodeHandle nh;

    ROS_UART_Tx tx("/dev/ttyAMA0", "/rover/uart/tx", &nh);

    ros::spin();
}
