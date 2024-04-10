#include "nav_core/ImuGpsReceive.hpp"

int main(int argc, char** argv) {
  ros::init(argc, argv, "imu_gps_node");
  ImuGpsReceiveSerial imu_gps;
  ros::spin();
  return 0;
}