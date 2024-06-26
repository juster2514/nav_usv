#ifndef SBUS_SIMULATE_HPP
#define SBUS_SIMULATE_HPP

#include <qserialport.h>
#include <qthread.h>
#include <ros/ros.h>
#include <unistd.h>

#include <algorithm>
#include <memory>
#include <thread>
#include <functional>

#include "nav_core/sbus_channels_msg.h"

#define SBUS_FRAME_SIZE 25
#define SBUS_RANGE_MIN 200.0f
#define SBUS_RANGE_MAX 1800.0f
#define SBUS_TARGET_MIN 1000.0f
#define SBUS_TARGET_MAX 2000.0f
#define SBUS_SCALE_FACTOR                \
  ((SBUS_TARGET_MAX - SBUS_TARGET_MIN) / \
   (SBUS_RANGE_MAX - SBUS_RANGE_MIN))  // 0.625f
#define SBUS_SCALE_OFFSET \
  (int)(SBUS_TARGET_MIN - \
        (SBUS_SCALE_FACTOR * SBUS_RANGE_MIN + 0.5f))  // 874.5f

class SbusSimulateSerial : public QObject {
  Q_OBJECT
 public:
    SbusSimulateSerial();
    ~SbusSimulateSerial() = default;
    bool InitSbusSimulateSerialPort(const std::string port_name) const;
    const qint16 SbusSimulateOutput(const uint16_t channels_num) const;
    void SubSbusNewChannelsValueCallback(const nav_core::sbus_channels_msg::ConstPtr& msg) {
      for (size_t i{0}; i < msg->channels_value.size(); i++){
         this->values_.at(i) =msg->channels_value[i];
      }
      ROS_INFO_STREAM("ch1:"<<msg->channels_value[0]);
      ROS_INFO_STREAM("ch2:"<<msg->channels_value[1]);
    }

 public:
    void SbusOutputThread();

 private:
    ros::NodeHandle nh_;
    std::array<uint16_t, 16> values_{1700};
    ros::Subscriber sub_sbus_channels_value_;
    std::shared_ptr<QSerialPort> sbus_simulate_ser_;
};

#endif  // SBUS_SIMULATE_HPP