#include "nav_core/NavUSVcontrol.hpp"
#include "nav_core/remote_control_msg.h"
#include "nav_core/pos_vel_att_msg.h"

NavUSVcontrol::NavUSVcontrol():nh_("~"){
    pub_sbus_channels_value_ =nh_.advertise<nav_core::sbus_channels_msg>("sbus_channel_values", 10);
    sub_remote_ctrl_ = nh_.subscribe<nav_core::remote_control_msg>("/remote_data_node/remote_ctrl_data",10,&NavUSVcontrol::RemoteCallback,this);
    path_statue_ = nh_.subscribe<nav_core::pos_vel_att_msg>("/ekf_nav_update_node/run_status_data",10,&NavUSVcontrol::PathPlan,this);
}

void NavUSVcontrol::RemoteCallback(const nav_core::remote_control_msg::ConstPtr & msg){
    nav_core::sbus_channels_msg sbus_output_data;
    int16_t power = msg->ch_1,ch1;
    int16_t direction = msg->ch_2,ch2;
    int16_t  K = 200;
    power = 1000+(msg->ch_1-200)/1.6;
    direction = 1-(msg->ch_2-200)/800;
    ch1 = power + K*direction;
    CheckChvalue(ch1);
    ch2 = power + K*direction;
    CheckChvalue(ch2);
    sbus_output_data.channels_value[0]=ch1;
    sbus_output_data.channels_value[1]=ch2;
    pub_sbus_channels_value_.publish(sbus_output_data);
}

void NavUSVcontrol::CheckChvalue(int16_t &ch){
    if (ch >=2000)ch = 2000;
    else if (ch <=1000)ch = 1000;
}

void NavUSVcontrol::PathPlan(const nav_core::pos_vel_att_msg::ConstPtr &msg){
    double x,y,distance,angle;
    x=position_first.pose.position.x-msg->pos.pose.position.x;
    y=position_first.pose.position.y-msg->pos.pose.position.y;
    distance = sqrt(pow(x,2)+pow(y,2));
    angle = atan2(y,x);
}