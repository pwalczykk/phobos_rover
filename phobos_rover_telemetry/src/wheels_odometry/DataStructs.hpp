#ifndef DATASTRUCTS_HPP_
#define DATASTRUCTS_HPP_

struct DataEncoders{
    int wheel_imps_fl;
    int wheel_imps_fr;
    int wheel_imps_ml;
    int wheel_imps_mr;
    int wheel_imps_bl;
    int wheel_imps_br;
};

struct DataOdometry{
    float position_x;
    float position_y;
    float position_z;

    float orientation_x;
    float orientation_y;
    float orientation_z;
    float orientation_w;
};

#endif
