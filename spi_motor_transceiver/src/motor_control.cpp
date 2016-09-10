#include <string>
#include <iostream>
#include <sstream>
#include "../include/SPI_Master.hpp"
#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/UInt8.h"
#include <phobos_shared/ArmVel16.h>
#include <phobos_shared/WheelsVel16.h>
#include <phobos_shared/EncodersRockerBogie.h>
#include <phobos_shared/EncodersArm.h>
#include <phobos_shared/EncodersWheels.h>
#include <phobos_shared/EncodersRockerBogie.h>
#include <spi_motor_transceiver/WheelsStat6.h>
#include <spi_motor_transceiver/ArmStat6.h>
#include <phobos_shared/WheelsPID.h>

using namespace std;

#define LOOP_SPEED 100
#define PUBLISH_DIVISOR 10

SPI_Master spi_master;
ros::Publisher encoders;
ros::Publisher armEncoders;
ros::Publisher rockerBogieEncoders;
ros::Publisher wheelsStatus;
ros::Publisher armStatus;
int shouldPub = 0;

void wait_command() {
    uint8_t input[1] = { 0x0 }, output[1] = { 0x0 };
    while (ros::ok()) {
    	spi_master.WriteData(input, output, 1);
    	if (output[0] == 0x73) return;
    }
}

void setMotorsGetEncoders(const phobos_shared::WheelsVel16::ConstPtr& msg) {
    // Get values
    uint16_t w1 = msg->wheels_left;
    uint16_t w2 = msg->wheels_left;
    uint16_t w3 = msg->wheels_left;
    uint16_t w4 = msg->wheels_right;
    uint16_t w5 = msg->wheels_right;
    uint16_t w6 = msg->wheels_right;

    // Split to bytes
    uint8_t M1 = (w1 >> 8) & 0xFF;
    uint8_t L1 = w1 & 0xFF;
    uint8_t M2 = (w2 >> 8) & 0xFF;
    uint8_t L2 = w2 & 0xFF;
    uint8_t M3 = (w3 >> 8) & 0xFF;
    uint8_t L3 = w3 & 0xFF;
    uint8_t M4 = (w4 >> 8) & 0xFF;
    uint8_t L4 = w4 & 0xFF;
    uint8_t M5 = (w5 >> 8) & 0xFF;
    uint8_t L5 = w5 & 0xFF;
    uint8_t M6 = (w6 >> 8) & 0xFF;
    uint8_t L6 = w6 & 0xFF;

    // Debug - print original & split values
    cout << "m1: " << (int)w1 << " (" << (int)M1 << "/" << (int)L1;
    cout << ") m2: " << (int)w2 << " (" << (int)M2 << "/" << (int)L2;
    cout << ") m3: " << (int)w3 << " (" << (int)M3 << "/" << (int)L3;
    cout << ") m4: " << (int)w4 << " (" << (int)M4 << "/" << (int)L4;
    cout << ") m5: " << (int)w5 << " (" << (int)M5 << "/" << (int)L5;
    cout << ") m6: " << (int)w6 << " (" << (int)M6 << "/" << (int)L6 << ")" << endl;

    // Calculate CRC
    uint8_t CRC = (0x0C ^ 0x4D ^ 0x61 ^ M1 ^ L1 ^ M2 ^ L2 ^ M3 ^ L3 ^ M4 ^ L4 ^ M5 ^ L5 ^ M6 ^ L6);

    // Send command (1)
	uint8_t txBuffer1[18] = {0x73, 0x0C, 0x4D, 0x61, M1, L1, M2, L2, M3, L3, M4, L4, M5, L5, M6, L6, CRC, 0x65};
    uint8_t rxBuffer1[18] = {0,};
    spi_master.WriteData(txBuffer1,rxBuffer1, 18);

    // Wait for command processing
    wait_command();

    // Read response (2)
    uint8_t txBuffer2[20] = {0,};
    uint8_t rxBuffer2[20] = {0,};
    spi_master.WriteData(txBuffer2,rxBuffer2, 20);

    // Get response variables
    uint16_t eM1 = rxBuffer2[4];
    uint16_t eL1 = rxBuffer2[5];
    uint16_t eM2 = rxBuffer2[6];
    uint16_t eL2 = rxBuffer2[7];
    uint16_t eM3 = rxBuffer2[8];
    uint16_t eL3 = rxBuffer2[9];
    uint16_t eM4 = rxBuffer2[10];
    uint16_t eL4 = rxBuffer2[11];
    uint16_t eM5 = rxBuffer2[12];
    uint16_t eL5 = rxBuffer2[13];
    uint16_t eM6 = rxBuffer2[14];
    uint16_t eL6 = rxBuffer2[15];
    uint16_t eMS = rxBuffer2[16];
    uint16_t eLS = rxBuffer2[17];

    // Combine 8-bit values to 16-bit
    uint16_t e1 = ((eM1 << 8) | eL1);
    uint16_t e2 = ((eM2 << 8) | eL2);
    uint16_t e3 = ((eM3 << 8) | eL3);
    uint16_t e4 = ((eM4 << 8) | eL4);
    uint16_t e5 = ((eM5 << 8) | eL5);
    uint16_t e6 = ((eM6 << 8) | eL6);
    uint16_t eS = ((eMS << 8) | eLS);

    // Save response to proper variable
    phobos_shared::EncodersWheels data;
    data.wheel_vel_bl = e1;
    data.wheel_vel_ml = e2;
    data.wheel_vel_fl = e3;
    data.wheel_vel_fr = e4;
    data.wheel_vel_mr = e5;
    data.wheel_vel_br = e6;
    std_msgs::UInt8 sdata;
    sdata.data = eS;

    // Debug - print data
    cout << "e1: " << (int)data.wheel_vel_bl << " e2: " << (int)data.wheel_vel_ml;
    cout << " e3: " << (int)data.wheel_vel_fl << " e4: " << (int)data.wheel_vel_fr;
    cout << " e5: " << (int)data.wheel_vel_mr << " e6: " << (int)data.wheel_vel_br;
    cout << " S: " << (int)sdata.data << endl << endl;

    // Publish values
    encoders.publish(data);
    wheelsStatus.publish(sdata);
}


spi_motor_transceiver::WheelsStat6 getTemperatures() {
    // Transmit & receive buffer (Part 1)
    uint8_t txBuffer1[18] = {0x73, 0x0C, 0x4D, 0x62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x23, 0x65};
    uint8_t rxBuffer1[18] = {0,};

    // Send command
    spi_master.WriteData(txBuffer1,rxBuffer1, 18);

    // Wait for command processing
    wait_command();

    // Read response (2)
    uint8_t txBuffer2[20] = {0,};
    uint8_t rxBuffer2[20] = {0,};
    spi_master.WriteData(txBuffer2,rxBuffer2, 20);

    // Get response variables
    uint8_t m1 = rxBuffer2[4];
    uint8_t p1 = rxBuffer2[5];
    uint8_t m2 = rxBuffer2[6];
    uint8_t p2 = rxBuffer2[7];
    uint8_t m3 = rxBuffer2[8];
    uint8_t p3 = rxBuffer2[9];
    uint8_t m4 = rxBuffer2[10];
    uint8_t p4 = rxBuffer2[11];
    uint8_t m5 = rxBuffer2[12];
    uint8_t p5 = rxBuffer2[13];
    uint8_t m6 = rxBuffer2[14];
    uint8_t p6 = rxBuffer2[15];
    // uint8_t B1 = rxBuffer2[16]; // not used
    // uint8_t B2 = rxBuffer2[17]; // not used

    // Save response to proper variable
    spi_motor_transceiver::WheelsStat6 data;
    data.m1temp = m1;
    data.p1temp = p1;
    data.m2temp = m2;
    data.p2temp = p2;
    data.m3temp = m3;
    data.p3temp = p3;
    data.m4temp = m4;
    data.p4temp = p4;
    data.m5temp = m5;
    data.p5temp = p5;
    data.m6temp = m6;
    data.p6temp = p6;

    // Debug - print data
    cout << "m1: " << (int)data.m1temp << " p1: " << (int)data.p1temp;
    cout << " m2: " << (int)data.m2temp << " p2: " << (int)data.p2temp;
    cout << " m3: " << (int)data.m3temp << " p3: " << (int)data.p3temp;
    cout << " m4: " << (int)data.m4temp << " p4: " << (int)data.p4temp;
    cout << " m5: " << (int)data.m5temp << " p5: " << (int)data.p5temp;
    cout << " m6: " << (int)data.m6temp << " p6: " << (int)data.p6temp << endl << endl;

    // Return values
    return data;
}


void setArmGetEncoders(const phobos_shared::ArmVel16::ConstPtr& msg) {
    // Get values
    uint16_t l1 = msg->link_0;
    uint16_t l2 = msg->link_1;
    uint16_t l3 = msg->link_2;
    uint16_t l4 = msg->link_3;
    uint16_t l5 = msg->link_4;
    uint16_t l6 = msg->gripper;

    // Split to bytes
    uint8_t M1 = (l1 >> 8) & 0xFF;
    uint8_t L1 = l1 & 0xFF;
    uint8_t M2 = (l2 >> 8) & 0xFF;
    uint8_t L2 = l2 & 0xFF;
    uint8_t M3 = (l3 >> 8) & 0xFF;
    uint8_t L3 = l3 & 0xFF;
    uint8_t M4 = (l4 >> 8) & 0xFF;
    uint8_t L4 = l4 & 0xFF;
    uint8_t M5 = (l5 >> 8) & 0xFF;
    uint8_t L5 = l5 & 0xFF;
    uint8_t M6 = (l6 >> 8) & 0xFF;
    uint8_t L6 = l6 & 0xFF;

    // Debug - print original & split values
    cout << "l1: " << (int)l1 << " (" << (int)M1 << "/" << (int)L1;
    cout << ") l2: " << (int)l2 << " (" << (int)M2 << "/" << (int)L2;
    cout << ") l3: " << (int)l3 << " (" << (int)M3 << "/" << (int)L3;
    cout << ") l4: " << (int)l4 << " (" << (int)M4 << "/" << (int)L4;
    cout << ") l5: " << (int)l5 << " (" << (int)M5 << "/" << (int)L5;
    cout << ") l6: " << (int)l6 << " (" << (int)M6 << "/" << (int)L6 << ")" << endl;

    // Calculate CRC
    uint8_t CRC = (0x0C ^ 0x4D ^ 0x63 ^ M1 ^ L1 ^ M2 ^ L2 ^ M3 ^ L3 ^ M4 ^ L4 ^ M5 ^ L5 ^ M6 ^ L6);

    // Send command (1)
	uint8_t txBuffer1[18] = {0x73, 0x0C, 0x4D, 0x63, M1, L1, M2, L2, M3, L3, M4, L4, M5, L5, M6, L6, CRC, 0x65};
    uint8_t rxBuffer1[18] = {0,};
    spi_master.WriteData(txBuffer1,rxBuffer1, 18);

    // Wait for command processing
    wait_command();

    // Read response (2)
    uint8_t txBuffer2[20] = {0,};
    uint8_t rxBuffer2[20] = {0,};
    spi_master.WriteData(txBuffer2,rxBuffer2, 20);

    // Get response variables
    uint16_t eM1 = rxBuffer2[4];
    uint16_t eL1 = rxBuffer2[5];
    uint16_t eM2 = rxBuffer2[6];
    uint16_t eL2 = rxBuffer2[7];
    uint16_t eM3 = rxBuffer2[8];
    uint16_t eL3 = rxBuffer2[9];
    uint16_t eM4 = rxBuffer2[10];
    uint16_t eL4 = rxBuffer2[11];
    uint16_t eM5 = rxBuffer2[12];
    uint16_t eL5 = rxBuffer2[13];
    uint16_t eM6 = rxBuffer2[14];
    uint16_t eL6 = rxBuffer2[15];
    uint16_t eMS = rxBuffer2[16];
    uint16_t eLS = rxBuffer2[17];

    // Combine 8-bit values to 16-bit
    uint16_t e1 = ((eM1 << 8) | eL1);
    uint16_t e2 = ((eM2 << 8) | eL2);
    uint16_t e3 = ((eM3 << 8) | eL3);
    uint16_t e4 = ((eM4 << 8) | eL4);
    uint16_t e5 = ((eM5 << 8) | eL5);
    uint16_t e6 = ((eM6 << 8) | eL6);
    uint16_t eS = ((eMS << 8) | eLS);

    // Save response to proper variable
    phobos_shared::EncodersArm data;
    data.link_pose_0 = e1;
    data.link_pose_1 = e2;
    data.link_pose_2 = e3;
    data.link_pose_3 = e4;
    data.link_pose_4 = e5;
    data.grip_pose = e6;
    std_msgs::UInt8 sdata;
    sdata.data = eS;

    // Debug - print data
    cout << "e1: " << (int)data.link_pose_0 << " e2: " << (int)data.link_pose_1;
    cout << " e3: " << (int)data.link_pose_2 << " e4: " << (int)data.link_pose_3;
    cout << " e5: " << (int)data.link_pose_4 << " e6: " << (int)data.grip_pose;
    cout << " S: " << (int)sdata.data << endl << endl;

    // Publish values
    armEncoders.publish(data);
    armStatus.publish(sdata);
}


spi_motor_transceiver::ArmStat6 getArmLimits() {
    // Transmit & receive buffer (Part 1)
    uint8_t txBuffer1[18] = {0x73, 0x0C, 0x4D, 0x64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x25, 0x65};
    uint8_t rxBuffer1[18] = {0,};

    // Send command
    spi_master.WriteData(txBuffer1,rxBuffer1, 18);

    // Wait for command processing
    wait_command();

    // Read response (2)
    uint8_t txBuffer2[20] = {0,};
    uint8_t rxBuffer2[20] = {0,};
    spi_master.WriteData(txBuffer2,rxBuffer2, 20);

    // Get response variables
    uint8_t M1 = rxBuffer2[4];
    uint8_t m1 = rxBuffer2[5];
    uint8_t M2 = rxBuffer2[6];
    uint8_t m2 = rxBuffer2[7];
    uint8_t M3 = rxBuffer2[8];
    uint8_t m3 = rxBuffer2[9];
    uint8_t M4 = rxBuffer2[10];
    uint8_t m4 = rxBuffer2[11];
    uint8_t M5 = rxBuffer2[12];
    uint8_t m5 = rxBuffer2[13];
    uint8_t M6 = rxBuffer2[14];
    uint8_t m6 = rxBuffer2[15];
    // uint8_t B1 = rxBuffer2[16]; // not used
    // uint8_t B2 = rxBuffer2[17]; // not used

    // Combine 8-bit values to 16-bit
    uint16_t l1 = ((M1 << 8) | m1);
    uint16_t l2 = ((M2 << 8) | m2);
    uint16_t l3 = ((M3 << 8) | m3);
    uint16_t l4 = ((M4 << 8) | m4);
    uint16_t l5 = ((M5 << 8) | m5);
    uint16_t l6 = ((M6 << 8) | m6);

    // Save response to proper variable
    spi_motor_transceiver::ArmStat6 data;
    data.limit1 = l1;
    data.limit2 = l2;
    data.limit3 = l3;
    data.limit4 = l4;
    data.limit5 = l5;
    data.limit6 = l6;

    // Debug - print data
    cout << "l1: " << (int)data.limit1 << " l2: " << (int)data.limit2;
    cout << " l3: " << (int)data.limit3 << " l4: " << (int)data.limit4;
    cout << " l5: " << (int)data.limit5 << " l6: " << (int)data.limit6 << endl << endl;

    // Return values
    return data;
}


void calibrateArm(const std_msgs::Int8::ConstPtr& msg) {
    // Get values
    uint8_t f = msg->data;

    // Calculate CRC
    uint8_t CRC = (0x0C ^ 0x4D ^ 0x65 ^ f);

    // Transmit & receive buffer (Part 1)
    uint8_t txBuffer1[18] = {0x73, 0x0C, 0x4D, 0x65, f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CRC, 0x65};
    uint8_t rxBuffer1[18] = {0,};

    // Send command
    spi_master.WriteData(txBuffer1,rxBuffer1, 18);

    // Wait for command processing
    wait_command();

    // Read response (2)
    uint8_t txBuffer2[20] = {0,};
    uint8_t rxBuffer2[20] = {0,};
    spi_master.WriteData(txBuffer2,rxBuffer2, 20);
}


void resetSubsystem(const std_msgs::Int8::ConstPtr& msg) {
    // Get values
    uint8_t r = msg->data;

    // Calculate CRC
    uint8_t CRC = (0x0C ^ 0x4D ^ 0x66 ^ r);

    // Transmit & receive buffer (Part 1)
    uint8_t txBuffer1[18] = {0x73, 0x0C, 0x4D, 0x66, r, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CRC, 0x65};
    uint8_t rxBuffer1[18] = {0,};

    // Send command
    spi_master.WriteData(txBuffer1,rxBuffer1, 18);

    // Wait for command processing
    wait_command();

    // Read response (2)
    uint8_t txBuffer2[20] = {0,};
    uint8_t rxBuffer2[20] = {0,};
    spi_master.WriteData(txBuffer2,rxBuffer2, 20);
}


void setPIDs(const phobos_shared::WheelsPID::ConstPtr& msg) {
    // Get values
    uint8_t a = msg->addr;
    uint16_t Pp = msg->P;
    uint16_t Ii = msg->I;
    uint16_t Dd = msg->D;

    // Split to bytes
    uint8_t P = (Pp >> 8) & 0xFF;
    uint8_t p = Pp & 0xFF;
    uint8_t I = (Ii >> 8) & 0xFF;
    uint8_t i = Ii & 0xFF;
    uint8_t D = (Dd >> 8) & 0xFF;
    uint8_t d = Dd & 0xFF;

    // Debug - print original & split values
    cout << "a: " << (int)a;
    cout << " P: " << (int)Pp << " (" << (int)P << "/" << (int)p;
    cout << ") I: " << (int)Ii << " (" << (int)I << "/" << (int)i;
    cout << ") D: " << (int)Dd << " (" << (int)D << "/" << (int)d << ")" << endl;

    // Calculate CRC
    uint8_t CRC = (0x0C ^ 0x4D ^ 0x67 ^ a ^ P ^ p ^ I ^ i ^ D ^ d);

    // Send command (1)
    uint8_t txBuffer1[18] = {0x73, 0x0C, 0x4D, 0x67, a, P, p, I, i, D, d, 0x00, 0x00, 0x00, 0x00, 0x00, CRC, 0x65};
    uint8_t rxBuffer1[18] = {0,};
    spi_master.WriteData(txBuffer1,rxBuffer1, 18);

    // Wait for command processing
    wait_command();

    // Read response (2)
    uint8_t txBuffer2[20] = {0,};
    uint8_t rxBuffer2[20] = {0,};
    spi_master.WriteData(txBuffer2,rxBuffer2, 20);
}


int main(int argc, char** argv)
{
    // Open SPI bus
    spi_master.OpenBus();

    // Initialize ROS node
    ros::init(argc,argv,"motor_control");
    ros::NodeHandle n;
    ros::Rate loop_rate(LOOP_SPEED);

    // Open publish & subscribe topics
    // Wheel motors, encoders & status (temperatures)
    ros::Subscriber pwms = n.subscribe("/rover/control/wheels_vel", 1000, setMotorsGetEncoders);
    encoders = n.advertise<phobos_shared::EncodersWheels>("/rover/encoders/wheels_relative", 1000);
    ros::Publisher temperatures = n.advertise<spi_motor_transceiver::WheelsStat6>("/rover/other/temperatures", 1000);
    // Arm motors, encoders & status (limits)
    ros::Subscriber armPwms = n.subscribe("/rover/control/arm_vel", 1000, setArmGetEncoders);
    armEncoders = n.advertise<phobos_shared::EncodersArm>("/rover/encoders/arm_absolute", 1000);
    ros::Publisher armLimits = n.advertise<spi_motor_transceiver::ArmStat6>("/rover/other/arm_limits", 1000);
    // Arm calibrate & subsystem reset
    ros::Subscriber armCalibrate = n.subscribe("/rover/other/arm_calibrate", 1000, calibrateArm);
    ros::Subscriber subReset = n.subscribe("/rover/other/subsystem_reset", 1000, resetSubsystem);
    // Rocker-bogie encoders
    rockerBogieEncoders = n.advertise<phobos_shared::EncodersRockerBogie>("/rover/encoders/rocker_bogie_absolute", 1000);
    // Wheel motors PIDs
    ros::Subscriber wheelPIDs = n.subscribe("/rover/control/wheels_pid", 1000, setPIDs);
    // Controller status byte
    wheelsStatus = n.advertise<std_msgs::UInt8>("/rover/other/wheels_driver_status", 1000);
    armStatus = n.advertise<std_msgs::UInt8>("/rover/other/arm_driver_status", 1000);

    // Main loop
    while (ros::ok())
    {
    	// Limit status publishing to every nth loop (n = PUBLISH_DIVISOR)
        if (shouldPub == 0) {
	        // Publish wheel status
    	    spi_motor_transceiver::WheelsStat6 wstat;
        	wstat = getTemperatures();
        	temperatures.publish(wstat);

        	// Publish arm status
        	spi_motor_transceiver::ArmStat6 armL;
        	armL = getArmLimits();
        	armLimits.publish(armL);

        	// Publish rocker-bogie
        	phobos_shared::EncodersRockerBogie rbEnc;
        	rbEnc.rocker_pose_l = 0;
        	rbEnc.rocker_pose_r = 0;
        	rbEnc.bogie_pose_l = 0;
        	rbEnc.bogie_pose_r = 0;
        	rockerBogieEncoders.publish(rbEnc);
        }
        shouldPub = (shouldPub + 1) % PUBLISH_DIVISOR;

        // Handle subscribed topics
        ros::spinOnce();

        // Loop speed contorl
        loop_rate.sleep();
    }

    return 0;
}

