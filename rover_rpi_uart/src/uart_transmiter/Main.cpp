#include "UART_Tx.hpp"

#include <ros/ros.h>

int main(int argc, char** argv){

    ros::init(argc, argv, "uart_transmiter");
    ros::NodeHandle nh;

    UART_Tx uart_tx("/dev/ttyAMA0");

    ros::Rate loop_rate(1);

    while(ros::ok()){
        uart_tx.Transmit("Hello ");
        uart_tx.Transmit("World ");
        loop_rate.sleep();
    }

}
