cmake_minimum_required(VERSION 3.0.2)
project(nav_core)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_INSTALL_PREFIX}/include)
link_directories(${CMAKE_INSTALL_PREFIX}/bin)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  serial
  std_msgs
  message_generation
)
find_package(tf2 REQUIRED)
find_package(Qt5 COMPONENTS Widgets Core SerialPort REQUIRED)
find_package(cmake_modules REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(yaml-cpp REQUIRED)


add_message_files(
  FILES
  sbus_channels_msg.msg
  remote_control_msg.msg
  pos_vel_att_msg.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
  geometry_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES nav_core
 CATKIN_DEPENDS roscpp rospy std_msgs message_runtime serial
#  DEPENDS system_lib
)


include_directories(
  ${catkin_INCLUDE_DIRS}
)
include_directories(${EIGEN3_INCLUDE_DIR})
add_definitions(${EIGEN_DEFINITIONS})
include_directories(./include)
include_directories(${YAML_CPP_INCLUDE_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)


qt5_wrap_cpp(remote_data_node_header_SRC include/nav_core/RemoteControlParser.hpp)

add_executable(remote_data_node
node/RemoteDataNode.cpp
src/RemoteControlParser.cpp
${remote_data_node_header_SRC}
)
target_link_libraries(remote_data_node
  ${catkin_LIBRARIES}
  Qt5::Widgets
  Qt5::Core
  Qt5::SerialPort
)

qt5_wrap_cpp(sbus_simulate_node_header_SRC include/nav_core/SbusSimulate.hpp)

# sbus simulate node
add_executable(sbus_simulate_node
node/SbusSimulateOutputNode.cpp
src/SbusSimulate.cpp
${sbus_simulate_node_header_SRC}
)
target_link_libraries(sbus_simulate_node
  ${catkin_LIBRARIES}
  Qt5::Widgets
  Qt5::Core
  Qt5::SerialPort
)

qt5_wrap_cpp(imu_gps_node_header_SRC include/nav_core/ImuGpsReceive.hpp)

# imu gps node
add_executable(imu_gps_node
node/ImuGpsReceiveNode.cpp
src/ImuGpsReceive.cpp
${imu_gps_node_header_SRC}
)
target_link_libraries(imu_gps_node
  ${catkin_LIBRARIES}
  Qt5::Widgets
  Qt5::Core
  Qt5::SerialPort
)

add_executable(ekf_nav_update_node
node/EKFNavUpdateNode.cpp
src/EKFImuGnss.cpp
)
target_link_libraries(ekf_nav_update_node
  ${catkin_LIBRARIES}
  yaml-cpp
)

add_executable(nav_usv_core_control_node
node/NavUSVcontrolNode.cpp
src/NavUSVcontrol.cpp
)
target_link_libraries(nav_usv_core_control_node
  ${catkin_LIBRARIES}
  Qt5::Widgets
  Qt5::Core
  Qt5::SerialPort
)
