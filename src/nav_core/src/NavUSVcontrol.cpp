#include "nav_core/NavUSVcontrol.hpp"
#include "nav_core/remote_control_msg.h"
#include "nav_core/pos_vel_att_msg.h"

NavUSVcontrol::NavUSVcontrol():nh_("~"){
    pub_sbus_channels_value_ =nh_.advertise<nav_core::sbus_channels_msg>("sbus_channel_values", 10);
    sub_remote_ctrl_ = nh_.subscribe<nav_core::remote_control_msg>("/remote_data_node/remote_ctrl_data",10,&NavUSVcontrol::RemoteCallback,this);
    path_statue_ = nh_.subscribe<nav_core::pos_vel_att_msg>("/ekf_nav_update_node/run_status_data",10,&NavUSVcontrol::PathPlan,this);

    position_first.pose.position.x = 20;
    position_first.pose.position.y = 20;
    position_first.pose.position.z = 0;

    std::thread nav_control_thread = std::thread(&NavUSVcontrol::NavNodeThread,this);
    nav_control_thread.detach();
}

void NavUSVcontrol::RemoteCallback(const nav_core::remote_control_msg::ConstPtr & msg){

    if (msg->key_value == 200){
        decision = true;
        power = msg->ch_1;
        direction = msg->ch_2;
    }
    else if (msg ->key_value == 1800)decision = false;

}

void NavUSVcontrol::SbusRemoteOutput(int16_t power_,int16_t direction_){
    power = 1000+(power_-200)/1.6;
    direction = 1-(direction_-200)/800;

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
    double x,y;
    x=position_first.pose.position.x-msg->pos.pose.position.x;
    y=position_first.pose.position.y-msg->pos.pose.position.y;

    angle_realy = atan2(2*((msg->pos.pose.orientation.w * msg->pos.pose.orientation.z)+
    (msg->pos.pose.orientation.x *msg->pos.pose.orientation.y)),
    1-2*((msg->pos.pose.orientation.y*msg->pos.pose.orientation.y)+
    (msg->pos.pose.orientation.z*msg->pos.pose.orientation.z)));

    distance = sqrt(pow(x,2)+pow(y,2));
    angle_goal = atan2(y,x);

    angle_error = angle_realy-angle_error;
    angle_error_num = angle_error_num + angle_error;
    OutsideControl();

    Vl = Vc + (Wc*L)/2 +1500;
    Vr = Vc - (Wc*L)/2 +1500;
    
}

void NavUSVcontrol::SbusAutoOutput(double Vl_,double Vr_){
    sbus_output_data.channels_value[0]=Vl_;
    sbus_output_data.channels_value[1]=Vr_;
    pub_sbus_channels_value_.publish(sbus_output_data);
}

void NavUSVcontrol::OutsideControl(){
    Wc = Kpw*angle_error + Kiw*angle_error_num + Kdw*(angle_error-angle_error_pre);
    Vc = Kpv*distance;
}

void NavUSVcontrol::NavNodeThread(){
    while (ros::ok)
    {
        if (decision)SbusRemoteOutput(power,direction);
        else SbusAutoOutput(Vl,Vr);
        ros::spinOnce();
    }
}