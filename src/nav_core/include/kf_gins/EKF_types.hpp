#ifndef EKF_TYPES_HPP
#define EKF_TYPES_HPP

#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <vector>

typedef struct Attitude
{
    Eigen::Quaterniond Qbn;
    Eigen::Matrix3d Cbn;
    Eigen::Vector3d Euler;
} Attitude;

typedef struct PosVelAtt
{
    Eigen::Vector3d pos;
    Eigen::Vector3d vel;
    Attitude att;
} PosVelAtt;

typedef struct ImuError
{
    Eigen::Vector3d gyrbias;
    Eigen::Vector3d accbias;
    Eigen::Vector3d gyrscale;
    Eigen::Vector3d accscale;
} ImuError;

typedef struct NavState
{
    Eigen::Vector3d pos;
    Eigen::Vector3d vel;
    Eigen::Vector3d euler;

    ImuError imuerror;
} NavState;

typedef struct ImuNoise {
    Eigen::Vector3d gyr_arw;
    Eigen::Vector3d acc_vrw;
    Eigen::Vector3d gyrbias_std;
    Eigen::Vector3d accbias_std;
    Eigen::Vector3d gyrscale_std;
    Eigen::Vector3d accscale_std;

    double corr_time;
} ImuNoise;

typedef struct EKFOption
{
    NavState initstate;
    NavState initstate_std;
    ImuNoise imunoise;

    Eigen::Vector3d antlever = {0, 0, 0};
    
} EKFOption;

typedef struct IMU
{
    double time;
    double dt;

    Eigen::Vector3d dangle;
    Eigen::Vector3d dvel;

} IMU;

typedef struct GNSS
{
    double time;

    Eigen::Vector3d llh;
    Eigen::Vector3d std;

} GNSS;

typedef struct Kalman
{
    Eigen::MatrixXd Cov_;
    Eigen::MatrixXd Qc_;
    Eigen::MatrixXd dx_;
} Kalman;


#endif