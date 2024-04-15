/***************************************
*日期：2024-4-13
*描述：接收遥控器的SBUS信号，将其发布给控制核心
***************************************/
#include "nav_core/RemoteControlParser.hpp"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>

/*
*描述：类RemoteControlDataParser的构造函数
*作用：初始化类的成员，开始遥控器信号接收线程
*参数: 无
*输出：无
*/
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
/*
*描述：遥控器接收串口初始化
*作用：使用QT串口库对串口进行初始化设置并打开串口
*参数：port_name:要打开的串口名
*输出：打开串口是否成功
*/
bool RemoteControlDataParser::InitRemoteCtrlSerialport(const std::string port_name)const{
    remote_control_serial_->setPortName(QString::fromStdString(port_name));
    remote_control_serial_->setBaudRate(100000);
    remote_control_serial_->setDataBits(QSerialPort::Data8);
    remote_control_serial_->setParity(QSerialPort::EvenParity);
    remote_control_serial_->setStopBits(QSerialPort::TwoStop);
    remote_control_serial_->setFlowControl(QSerialPort::NoFlowControl);
    if (remote_control_serial_->open(QIODevice::ReadWrite)) {
        ROS_INFO_STREAM("Open Remote Receive Serial Success in: "<<remote_control_serial_->portName().toStdString());
        return true;
    }else{
        ROS_INFO_STREAM("Open Remote Receive Serial Failed in: "<< port_name);
        return false;
    }
}
/*
*描述：订阅接收机SBUS信号的回调函数
*作用：对每一次接收机的SBUS信号做处理
*参数：无
*输出：无
*/

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

/*
*描述：解析接收机接收的SBUS信号
*作用：将SBUS信号对应通道解析好的值存入自定义的消息类型中
*参数：sbus_buf:接收机的初始数据
      rc_ctrl:解析后放入的自定义消息类型
*输出：无
*/
void RemoteControlDataParser::Parser(const QByteArray &sbus_buf, nav_core::remote_control_msg &rc_ctrl){
    auto sbus_buf_temp = reinterpret_cast<unsigned char *>(const_cast<char *>(sbus_buf.data()));
    rc_ctrl.ch_1 = static_cast<int16_t>((sbus_buf_temp[1] | (sbus_buf_temp[2] << 8)) & 0x07ff);                      // Channel 1 前进/后退
    rc_ctrl.ch_2 = static_cast<int16_t>(((sbus_buf_temp[2] >> 3) | (sbus_buf_temp[3] << 5)) & 0x07ff);               // Channel 2 左右转向
    rc_ctrl.key_value = static_cast<int16_t>(((sbus_buf_temp[6] >> 4) | (sbus_buf_temp[7] << 4)) & 0x07ff);          // 自动/手动模式切换
    rc_ctrl.k = static_cast<int16_t>(((sbus_buf_temp[7]>>7)|(sbus_buf_temp[8]<<1)|(sbus_buf_temp[9]<<9)) & 0x07ff);  // 转向
}

