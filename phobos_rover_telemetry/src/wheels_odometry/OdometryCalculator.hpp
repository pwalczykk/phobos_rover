#ifndef ODOMETRYCALCULATOR_HPP_
#define ODOMETRYCALCULATOR_HPP_

#include "DataStructs.hpp"

// Uniform Matrix (Orientation 3x3, Position 1x3, Perspective 3x1, Scale 1x1) adressing:
/*
    [ 0   1   2   3]
    [ 4   5   6   7]
    [ 8   9  10  11]
    [12  13  14  15]

    for example displacement_x is: Matrix[3]
*/


class OdometryCalculcator{
    ros::NodeHandle* nh;

    float POSE[16];  // Pose recived from previous kalman iteration - used to calculate current pose matrix
    DataOdometry *ODOMETRY;

    float DISP[16];  // Displacement calculated from odometry readings
    DataEncoders *ENCODERS;

protected:
    float displacement;
public:
    int impulses_per_rotation;
    float wheel_circumference;
    float wheels_spacing;

public:
    OdometryCalculcator(DataOdometry *ODOMETRY, DataEncoders *ENCODERS){
        this->ODOMETRY = ODOMETRY;
        this->ENCODERS = ENCODERS;

        // Zeroing all matrixes
        for(int i = 0; i < 16; i++){
            *(POSE + i) = 0;
            *(DISP + i) = 0;
        }

        // Setting 1 on displacemet matrix's diagonal
        *(DISP + 0) = 1;
        *(DISP + 5) = 1;
        *(DISP + 10) = 1;
        *(DISP + 15) = 1;

        // Setting 1 on pose matrix's scale:
        *(POSE + 15) = 1;

    }

    void Update(){
        this->CalcDisplacement();
        this->DispToMatrix();
        this->PoseToMatrix();
        this->MatrixMultiply();
    }

    // Displacement is calculated as average of lowest wheel velocitiy from left and right side
    void CalcDisplacement(){
        // Lowest wheel velocity on left side
        float wheel_impulses_l = ENCODERS->wheel_imps_fl;
        if(ENCODERS->wheel_imps_ml < wheel_impulses_l) wheel_impulses_l = ENCODERS->wheel_imps_ml;
        if(ENCODERS->wheel_imps_bl < wheel_impulses_l) wheel_impulses_l = ENCODERS->wheel_imps_bl;

        // Lowest wheel velocity on right side
        float wheel_impulses_r = ENCODERS->wheel_imps_fr;
        if(ENCODERS->wheel_imps_mr < wheel_impulses_r) wheel_impulses_r = ENCODERS->wheel_imps_mr;
        if(ENCODERS->wheel_imps_br < wheel_impulses_r) wheel_impulses_r = ENCODERS->wheel_imps_br;

        float displacement_l = wheel_circumference * wheel_impulses_l / impulses_per_rotation;
        float displacement_r = wheel_circumference * wheel_impulses_r / impulses_per_rotation;

        this->displacement = (displacement_l + displacement_r) / 2;
    }

    void DispToMatrix(){
        *(DISP + 3) = displacement;
    }

    // Basing on:
    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/
    void PoseToMatrix(){
        float qx2 = ODOMETRY->orientation_x * ODOMETRY->orientation_x;
        float qy2 = ODOMETRY->orientation_y * ODOMETRY->orientation_y;
        float qz2 = ODOMETRY->orientation_z * ODOMETRY->orientation_z;

        float qxqy = ODOMETRY->orientation_x * ODOMETRY->orientation_y;
        float qxqz = ODOMETRY->orientation_x * ODOMETRY->orientation_z;
        float qxqw = ODOMETRY->orientation_x * ODOMETRY->orientation_w;

        float qyqz = ODOMETRY->orientation_y * ODOMETRY->orientation_z;
        float qyqw = ODOMETRY->orientation_y * ODOMETRY->orientation_w;

        float qzqw = ODOMETRY->orientation_z * ODOMETRY->orientation_w;

        // Orientation (3x3)
        *(POSE +  0) = 1 - 2*qy2 - 2*qz2;
        *(POSE +  1) = 2*qxqy - 2*qzqw;
        *(POSE +  2) = 2*qxqz + 2*qyqw;

        *(POSE +  4) = 2*qxqy + 2*qzqw;
        *(POSE +  5) = 1 - 2*qx2 - 2*qz2;
        *(POSE +  6) = 2*qyqz - 2*qxqw;

        *(POSE +  8) = 2*qxqz - 2*qyqw;
        *(POSE +  9) = 2*qyqz + 2*qxqw;
        *(POSE + 10) = 1 - 2*qx2 - 2*qy2;

        // Displacement (1x3)
        *(POSE +  3) = ODOMETRY->position_x;
        *(POSE +  7) = ODOMETRY->position_y;
        *(POSE + 11) = ODOMETRY->position_z;

        // Perspective (3x1)
        // *(POSE + 12) = 0;
        // *(POSE + 13) = 0;
        // *(POSE + 14) = 0;

        //Scale (1x1)
        // *(POSE + 15) = 1;
    }

    //TODO : Można połączyć MatrixMultiply() i MatrixToMsgOdometry()

    // Version 1: Only displacement
    // As we assume no orientation changes in single iteration,
    // only displacement matrix (1x3) must be calcualted.
    // Results are assigned directly to global DataOdometry structure.
    // Basing on:
    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
    void MatrixMultiply(){
        ODOMETRY->position_x = *(POSE+0)**(DISP+3) + *(POSE+1)**(DISP+7) + *(POSE +2)**(DISP+11) + *(POSE+ 3)**(DISP+15);
        ODOMETRY->position_y = *(POSE+4)**(DISP+3) + *(POSE+5)**(DISP+7) + *(POSE +6)**(DISP+11) + *(POSE+ 7)**(DISP+15);
        ODOMETRY->position_z = *(POSE+8)**(DISP+3) + *(POSE+9)**(DISP+7) + *(POSE+10)**(DISP+11) + *(POSE+11)**(DISP+15);
    }
};

#endif
