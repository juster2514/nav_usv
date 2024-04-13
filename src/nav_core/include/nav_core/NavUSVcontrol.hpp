#ifndef NAVUSVCONTROL_HPP
#define NAVUSVCONTROL_HPP

#include "nav_core/sbus_channels_msg.h"
#include "nav_core/remote_control_msg.h"
#include "nav_core/pos_vel_att_msg.h"
#include <nav_msgs/Path.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/PoseStamped.h>
#include "SbusSimulate.hpp"

class NavUSVcontrol{
 public:
    NavUSVcontrol();
    ~NavUSVcontrol() = default;
   void RemoteCallback(const nav_core::remote_control_msg::ConstPtr& msg);
   void SbusRemoteOutput(int16_t power,int16_t direction);
   void SbusAutoOutput(double Vl_,double Vr_);
   void CheckChvalue(int16_t &ch);
   void PathPlan(const nav_core::pos_vel_att_msg::ConstPtr &msg);
   void OutsideControl();
   void NavNodeThread();
 private:
    ros::NodeHandle nh_;
    ros::Publisher pub_sbus_channels_value_;
    ros::Subscriber path_statue_;
    ros::Subscriber sub_remote_ctrl_;
    nav_core::sbus_channels_msg sbus_output_data;
    geometry_msgs::PoseStamped position_first;

    int16_t power,ch1;
    int16_t direction,ch2;
    int16_t K=200;

    double distance,angle_goal,angle_realy;
    double angle_error,angle_error_num,angle_error_pre = 0;
    double Wc,Vc,Vl,Vr,L;
    double Kpw=1,Kiw=0,Kdw=0.1,Kpv=1;
    bool decision;
};


#endif 