#include "nav_core/RemoteControlParser.hpp"


int main(int argc, char** argv) {
  QApplication a(argc, argv);
  ros::init(argc, argv, "remote_data_node");
  RemoteControlDataParser parser;
  return QApplication::exec();
} 