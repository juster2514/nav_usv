#include "nav_core/RemoteControlParser.hpp"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>

RemoteControlDataParser::RemoteControlDataParser(){
    ros::NodeHandle nh("~");
    remote_control_serial_ = std::make_shared<QSerialPort>();
    remote_ctrl_msg_pub = nh.advertise<nav_core::remote_control_msg>("remote_ctrl_data",1);
    InitRemoteCtrlSerialport("/dev/ttyUSB0");
    connect(remote_control_serial_.get(), SIGNAL(readyRead()), this,
          SLOT(ReadRemoteControlSerialDataCallback()));
    std::thread remote_ctl_thread = std::thread(&RemoteControlDataParser::NodeSpinThread, this);
    remote_ctl_thread.detach();

}

void RemoteControlDataParser::InitRemoteCtrlSerialport(const std::string port_name)const{
    remote_control_serial_->setPortName(QString::fromStdString(port_name));
    remote_control_serial_->setBaudRate(100000);
    remote_control_serial_->setDataBits(QSerialPort::Data8);
    remote_control_serial_->setParity(QSerialPort::EvenParity);
    remote_control_serial_->setStopBits(QSerialPort::TwoStop);
    remote_control_serial_->setFlowControl(QSerialPort::NoFlowControl);
    if (remote_control_serial_->open(QIODevice::ReadWrite)) {
    ROS_INFO_STREAM("Open SBUS Simulate Serial Success in: "<<remote_control_serial_->portName().toStdString());
    }
}

void RemoteControlDataParser::Parser(const QByteArray &sbus_buf, nav_core::remote_control_msg &rc_ctrl){
    auto sbus_buf_temp = reinterpret_cast<unsigned char *>(const_cast<char *>(sbus_buf.data()));
    rc_ctrl.ch_1 = static_cast<int16_t>((sbus_buf_temp[1] | (sbus_buf_temp[2] << 8)) & 0x07ff);         // Channel 1 前进/后退
    rc_ctrl.ch_2 = static_cast<int16_t>(((sbus_buf_temp[2] >> 3) | (sbus_buf_temp[3] << 5)) & 0x07ff);  // Channel 2 左右转向
    rc_ctrl.key_value = static_cast<int16_t>(((sbus_buf_temp[6] >> 4) | (sbus_buf_temp[7] << 4)) & 0x07ff);
}

void RemoteControlDataParser::ReadRemoteControlSerialDataCallback(){
    qint64 rece_len = remote_control_serial_->bytesAvailable();
    QByteArray received_buffer = remote_control_serial_->readAll();
    if (!received_buffer.isEmpty())
    {
        nav_core::remote_control_msg remote_ctl_msg;
        remote_ctl_msg.header.stamp = ros::Time::now();
        Parser(received_buffer,remote_ctl_msg);
        remote_ctrl_msg_pub.publish(remote_ctl_msg);
    }
    
}