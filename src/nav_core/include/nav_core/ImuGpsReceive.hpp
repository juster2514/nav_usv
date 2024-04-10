#ifndef IMUGPSRECEIVE_HPP
#define IMUGPSRECEIVE_HPP

#include <glog/logging.h>
#include <ros/ros.h>
#include <unistd.h>
#include "serial/serial.h"
#include <tf2/LinearMath/Quaternion.h>
#include <algorithm>
#include <memory>
#include <thread>

#include <sensor_msgs/Imu.h>
#include <sensor_msgs/NavSatFix.h>


class ImuGpsReceiveSerial{
 public:
    ImuGpsReceiveSerial();
    ~ImuGpsReceiveSerial(){};
    bool InitImuGpsReceiveSerialPort(const std::string port_name) const;
    void ImuGpsRead();
    void ImuGpsThread();
    bool checkSum(const std::vector<uint8_t> &data_buff);
    std::vector<int16_t> hexToShort(const std::vector<uint8_t> &hex_data);
    std::vector<int32_t> hexToLong(const std::vector<uint8_t> &hex_data);
 private:
    ros::NodeHandle nh_;

    ros::Publisher imu_analysis_value_;
    ros::Publisher gps_anglysis_value_;

    sensor_msgs::Imu imu_msg;
    sensor_msgs::NavSatFix gps_msg;

    std::vector<int16_t> acceleration;
    std::vector<int16_t> angularVelocity;
    std::vector<int16_t> angle_degree;
    std::vector<int32_t> lon_lat;
    std::vector<int16_t> gps_data;

    std::atomic_bool imu_data_ready;
    std::atomic_bool gps_data_ready;

    std::shared_ptr<serial::Serial> imu_gps_ser_;
    std::shared_ptr<tf2::Quaternion> quat;
};

#endif