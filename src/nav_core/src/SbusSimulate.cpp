/****************************************************
*日期:2024-03-10
*描述：接收控制电机的pwm信号，生成SBUS信号，并将其发布从而控制电机
****************************************************/
#include "nav_core/SbusSimulate.hpp"

/*
*描述：类SbusSimulateSerial的构造函数
*作用：初始化类的成员，开启SBUS信号输出线程
*参数：无
*输出：无
*/
SbusSimulateSerial::SbusSimulateSerial() : nh_("~") {
  sbus_simulate_ser_ = std::make_shared<QSerialPort>();
  InitSbusSimulateSerialPort(
      "/dev/ttyUSB1");
  sub_sbus_channels_value_ = nh_.subscribe<nav_core::sbus_channels_msg>(
      "/nav_usv_core_control_node/sbus_channel_values", 100, &SbusSimulateSerial::SubSbusNewChannelsValueCallback, this);
  SbusOutputThread();
}

/*
*描述：输出SBUS信号的串口初始化函数
*作用：通过QT的串口类对串口初始化设置以及打开串口
*参数：port_name:要打开的串口名
*输出：打开串口是否成功
*/
bool SbusSimulateSerial::InitSbusSimulateSerialPort(
    const std::string port_name) const {
  sbus_simulate_ser_->setPortName(QString::fromStdString(port_name));
  sbus_simulate_ser_->setBaudRate(100000);
  sbus_simulate_ser_->setDataBits(QSerialPort::Data8);
  sbus_simulate_ser_->setParity(QSerialPort::EvenParity);
  sbus_simulate_ser_->setStopBits(QSerialPort::TwoStop);
  sbus_simulate_ser_->setFlowControl(QSerialPort::NoFlowControl);
  if (sbus_simulate_ser_->open(QIODevice::ReadWrite)) {
    ROS_INFO_STREAM("Open SBUS Simulate Serial Success in: "<<sbus_simulate_ser_->portName().toStdString());
    return true;
  } else {
    ROS_INFO_STREAM("Open SBUS Simulate Serial Failed in: "<< port_name);
    return false;
  }
}

/*
*描述：SBUS线程函数
*作用：开启SBUS线程，开始循环发送SBUS至串口
*参数：无
*输出：无
*/
void SbusSimulateSerial::SbusOutputThread() {
  while (ros::ok()) {
    this->SbusSimulateOutput(16);
    ros::spinOnce();
  }
}

/*
*描述：SBUS信号合成及发送函数
*作用：将需要生成的16个通道的PWM合成为1个SBUS信号，并发送至串口
*参数：channels_num:通道数（默认为16）
*输出：无
*/
const qint16 SbusSimulateSerial::SbusSimulateOutput(
    const uint16_t channels_num) const {
  int i = 0;
  uint16_t value = 0;
  uint8_t byteindex = 1;
  uint8_t offset = 0;
  uint8_t oframe[25] = {0};
  memset(oframe, 0, 25);
  oframe[0] = 0x0f;
  oframe[24] = 0x00;

  for (i = 0; (i < channels_num) && (i < 16); ++i) {
    value = (unsigned short)(((values_[i] - SBUS_SCALE_OFFSET) /
                              SBUS_SCALE_FACTOR) +
                             .5f);
    if (value > 0x07ff) {
      value = 0x07ff;
    }

    while (offset >= 8) {
      ++byteindex;
      offset -= 8;
    }

    oframe[byteindex] |= (value << (offset)) & 0xff;
    oframe[byteindex + 1] |= (value >> (8 - offset)) & 0xff;
    oframe[byteindex + 2] |= (value >> (16 - offset)) & 0xff;
    offset += 11;
  }
  qint16 write_len = sbus_simulate_ser_->write((const char *)oframe, 25);
  sbus_simulate_ser_->waitForBytesWritten(1);       //注意此处等待写入延时和下一行线程延时总时长必须为7ms！！！（具体原因查看SBUS协议规定）
  QThread::msleep(6);
  
}


