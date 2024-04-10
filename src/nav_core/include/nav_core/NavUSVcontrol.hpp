#ifndef NAVUSVCONTROL_HPP
#define NAVUSVCONTROL_HPP

#include "nav_core/sbus_channels_msg.h"
#include "nav_core/remote_control_msg.h"
#include <nav_msgs/Path.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/PoseStamped.h>
#include "SbusSimulate.hpp"

class NavUSVcontrol{
 public:
    NavUSVcontrol();
    ~NavUSVcontrol() = default;
   void RemoteCallback(const nav_core::remote_control_msg::ConstPtr& msg);
   void CheckChvalue(int16_t &ch);
   void PathPlan(const nav_core::pos_vel_att_msg::ConstPtr &msg);
 private:
    ros::NodeHandle nh_;
    ros::Publisher pub_sbus_channels_value_;
    ros::Subscriber path_statue_;
    ros::Subscriber sub_remote_ctrl_;
    
    geometry_msgs::PoseStamped position_first;
};


#endif 