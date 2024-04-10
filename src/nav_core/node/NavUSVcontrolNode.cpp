#include "nav_core/NavUSVcontrol.hpp"

int main(int argc, char** argv) {
  ros::init(argc, argv, "nav_usv_core_control_node");
  NavUSVcontrol nav_usv_core_control;
  ros::spin();
  return 0;
}