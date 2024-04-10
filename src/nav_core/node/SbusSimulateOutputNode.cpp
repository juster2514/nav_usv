#include "nav_core/SbusSimulate.hpp"
#include "qapplication.h"

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  ros::init(argc, argv, "sbus_simulate_node");
  SbusSimulateSerial sbus_simu_ser;
  return 0;
}
