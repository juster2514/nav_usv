#ifndef NAVUSVCONTROL_HPP
#define NAVUSVCONTROL_HPP

#include "nav_core/sbus_channels_msg.h"
#include "nav_core/remote_control_msg.h"
#include "SbusSimulate.hpp"

class NavUSVcontrol{
 public:
    NavUSVcontrol();
    ~NavUSVcontrol() = default;
   void RemoteCallback(const nav_core::remote_control_msg::ConstPtr& msg);
   void CheckChvalue(int16_t &ch);
 private:
    ros::NodeHandle nh_;
    ros::Publisher pub_sbus_channels_value_;
    ros::Subscriber sub_remote_ctrl_;
};


#endif 