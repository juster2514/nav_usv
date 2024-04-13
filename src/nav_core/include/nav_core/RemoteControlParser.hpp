#ifndef  REMOTECONTROLPARSER_HPP
#define REMOTECONTROLPARSER_HPP

#include <ros/ros.h>
#include <std_msgs/Header.h>

#include <QApplication>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <memory>
#include <thread>

#include "nav_core/remote_control_msg.h"


class RemoteControlDataParser : public QObject {
 Q_OBJECT
 public:
    RemoteControlDataParser();
    ~RemoteControlDataParser() = default;
    void NodeSpinThread() {
      ros::spin();
      ros::shutdown();
    }
 public slots:
    void ReadRemoteControlSerialDataCallback();
 private:
    void Parser(const QByteArray &sbus_buf,nav_core::remote_control_msg &rc_ctrl);
    bool InitRemoteCtrlSerialport(const std::string port_name)const;
    
    std::shared_ptr<QSerialPort> remote_control_serial_;
    ros::Publisher remote_ctrl_msg_pub;
};


#endif