#ifndef EKFIMUGNSS_HPP
#define EKFIMUGNSS_HPP

#include <Eigen/Eigen>
#include "EKF_types.hpp"
#include <ros/ros.h>

#include "nav_core/pos_vel_att_msg.h"

#include <sensor_msgs/Imu.h>
#include <sensor_msgs/NavSatFix.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/PoseStamped.h>


class EKFImuGnss{
 public:
    EKFImuGnss(EKFOption &options);
    ~EKFImuGnss()=default;
    void InitEKFImuGnss(const NavState &initstate, const NavState &initstate_std);
    void addImuData(const IMU &imu, bool compensate);
    void addGnssData(const GNSS &gnss);
    void ImuCompensate(IMU &imu);
    void newImuProcess();
    int isToUpdate(double imutime1, double imutime2, double updatetime) const;
    void ImuPropagation(IMU &imuprediction, IMU &imucurrent);
    void GnssUpdate(GNSS &gnssdate);
    void EKfPredict(Eigen::MatrixXd &Phi, Eigen::MatrixXd &Qd);
    void EKFUpdate(Eigen::MatrixXd &dz, Eigen::MatrixXd &H, Eigen::MatrixXd &R);
    void StateFeedback(); 
    void PosAtt2Path();
    void LLA2XYZ(std::vector<double> &llh,std::vector<double> &xyz);
    NavState GetNavState();
    Eigen::MatrixXd GetCov(){
      return kf_.Cov_;
    };
    void ImuCallback(const sensor_msgs::ImuConstPtr &imu_msg);
    void GpsCallback(const sensor_msgs::NavSatFixConstPtr &gps_msg);

   static void imuInterpolate(const IMU &imu1, IMU &imu2, const double timestamp, IMU &midimu) {
      if (imu1.time > timestamp || imu2.time < timestamp) {
         return;
      }
      double lamda = (timestamp - imu1.time) / (imu2.time - imu1.time);
      midimu.time   = timestamp;
      midimu.dangle = imu2.dangle * lamda;
      midimu.dvel   = imu2.dvel * lamda;
      midimu.dt     = timestamp - imu1.time;
      imu2.dangle = imu2.dangle - midimu.dangle;
      imu2.dvel   = imu2.dvel - midimu.dvel;
      imu2.dt     = imu2.dt - midimu.dt;
   }
 private:
    ros::NodeHandle nh_;
    ros::Subscriber sub_imu_data_;
    ros::Subscriber sub_gps_data_;
    ros::Publisher  pub_path_;
    ros::Publisher  run_status_;

    EKFOption options_;

    IMU imuprediction_;
    IMU imucurrent_;
    GNSS gnssdata_;

    nav_core::pos_vel_att_msg run_statue_msg;
    geometry_msgs::PoseStamped pos_msg;
    nav_msgs::Path path_msg;

    PosVelAtt pvaprediction_;
    PosVelAtt pvacurrent_;
    ImuError imuerror_;

    Kalman kf_;

    const int RANK      =21;
    const int NOISERANK =18;

    double timestamp_;
    bool inittime_flag;
    const double TIME_ALIGN_ERR = 0.012;
    std::vector<double> llh_pre,xyz_pre;
    temp_IMU temp_pre,temp_cur;

    enum StateID {P_ID=0,V_ID=3,ATT_ID=6,BG_ID=9,BA_ID=12,SG_ID=15,SA_ID=18};
    enum NoiseID {VRW_ID=0,ARW_ID=3,BGSTD_ID=6,BASTD_ID=9,SGSTD_ID=12,SASTD_ID=15};
    
};

#endif