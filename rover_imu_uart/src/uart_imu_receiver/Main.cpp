#include "ROS_UART_IMU_Rx.hpp"

int main(int argc, char** argv){

    ros::init(argc, argv, "uart_imu_receiver");
    ros::NodeHandle nh;

    ROS_UART_Rx rx("/dev/ttyACM3", "/imu/data_raw", "/imu/mag", &nh);

    ros::Rate loop_rate(10);

    while(ros::ok()){
        rx.CheckReceiveBuffer();
	ROS_INFO("SPRAWDZAM BUFOR");
        loop_rate.sleep();
    }
    return 0;
}
