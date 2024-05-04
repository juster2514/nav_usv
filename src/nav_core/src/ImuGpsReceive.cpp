/***************************
*日期：2024-3-12
*描述：通过串口获取IMU和GPS的原始数据并进行解析
***************************/
#include "nav_core/ImuGpsReceive.hpp"

/*
*描述：类ImuGpsReceiveSerial构造函数
*作用：生成一个serial::Serial对象,并初始化类中各成员，开启IMU和GPS数据接收的线程
*参数：无
*输出：无
*/
ImuGpsReceiveSerial::ImuGpsReceiveSerial():nh_("~"){

    imu_gps_ser_=std::make_shared<serial::Serial>();
    quat = std::make_shared<tf2::Quaternion>();

    std::vector<int16_t> acceleration(4,0);
    std::vector<int16_t> angularVelocity(4,0);
    std::vector<int16_t> angle_degree(4,0);
    std::vector<int32_t> lon_lat(2,0);
    std::vector<int16_t> gps_data(4,0);

    InitImuGpsReceiveSerialPort("/dev/ttyUSB0");

    imu_data_ready.store(false);
    gps_data_ready.store(false);

    imu_msg.header.frame_id = "imu_data"; 
    gps_msg.header.frame_id = "gps_data";
 
    imu_analysis_value_ =nh_.advertise<sensor_msgs::Imu>("imu_data", 100);
    gps_anglysis_value_ = nh_.advertise<sensor_msgs::NavSatFix>("gps_data",100);

    std::thread t_imu_gps =std::thread(&ImuGpsReceiveSerial::ImuGpsThread, this);
    t_imu_gps.detach();
}

/*
*描述：Imu和GPS接收线程
*作用：开始线程，循环接收IMU和GPS的数据
*参数：无
*输出：无
*/
void ImuGpsReceiveSerial::ImuGpsThread(){
    while (ros::ok()) {
        ImuGpsRead();
        if (imu_data_ready.load())
        {
            imu_analysis_value_.publish(imu_msg);
        }
        else if (gps_data_ready.load())
        {
            gps_anglysis_value_.publish(gps_msg);
        }
        imu_data_ready.store(false);
        gps_data_ready.store(false);
    }
}

/*
*描述：Imu和GPS串口初始化函数
*作用：使用ROS的串口库，对接收串口进行初始化并打开串口
*参数：port_name:接收的串口名
*输出：串口是否打开成功
*/
bool ImuGpsReceiveSerial::InitImuGpsReceiveSerialPort
    (const std::string port_name) const {
    imu_gps_ser_->setPort(port_name);
    imu_gps_ser_->setBaudrate(115200);
    serial::Timeout time_out =serial::Timeout::simpleTimeout(2000);
    imu_gps_ser_->setTimeout(time_out);
    imu_gps_ser_->open();
    if (imu_gps_ser_->isOpen()) {
        ROS_INFO_STREAM("Open IMU GPS Serial Success in: "<<imu_gps_ser_->getPort());
        return true;
    } else {
        ROS_INFO_STREAM("Open IMU GPS Serial Failed In: "<< port_name);
        return false;
    }
    imu_gps_ser_->flush();
}

/*
*描述：对Imu和GPS原始数据读取并进行解析
*作用：将Imu和GPS数据读取解析并以ROS的标准格式发布
*参数：无
*输出：无
*/
void ImuGpsReceiveSerial::ImuGpsRead(){

    if (imu_gps_ser_->available()){
        while (!imu_data_ready.load()&&!gps_data_ready.load())
        {
            imu_gps_ser_->read(&data_temp,1);
            buff.push_back(data_temp);
            if (buff.size() >=11 && buff[0] ==0x55){
                std::vector<uint8_t> initial_value(buff.begin(), buff.begin() + 11);
                std::vector<uint8_t> data(buff.begin() + 2, buff.begin() + 10);
                if (initial_value[1]==0x51)
                {
                    if (checkSum(initial_value))
                    {
                        acceleration = hexToShort(data);
                    }else{
                        ROS_INFO("Ox51 check error");
                    }
                    
                }
                else if (initial_value[1]==0x52)
                {
                    if (checkSum(initial_value))
                    {
                        angularVelocity = hexToShort(data);
                    }else{
                        ROS_INFO("Ox52 check error");
                    }
                }
                else if (initial_value[1]==0x53){
                    if (checkSum(initial_value))
                    {
                        angle_degree = hexToShort(data);
                        imu_data_ready.store(true);
                    }else{
                        ROS_INFO("Ox53 check error");
                    }
                }
                else if (initial_value[1]==0x57)
                {
                    if (checkSum(initial_value))
                    {
                        lon_lat = hexToLong(data);
                    }else{
                        ROS_INFO("Ox57 check error");
                    }
                }
                else if (initial_value[1]==0x58)
                {
                    if (checkSum(initial_value))
                    {
                        gps_data = hexToShort(data);
                        gps_data_ready.store(true);
                    }else{
                        ROS_INFO("Ox58 check error");
                    }
                }
                buff.clear();
            }else if (buff[0] != 0x55){
            buff.clear();  
            }
        }
        if (imu_data_ready.load())
        {
            imu_msg.linear_acceleration.x = static_cast<double>(acceleration[0]) / 32768.0 * 16 * 9.8;
            imu_msg.linear_acceleration.y = static_cast<double>(acceleration[1]) / 32768.0 * 16 * 9.8;
            imu_msg.linear_acceleration.z = static_cast<double>(acceleration[2]) / 32768.0 * 16 * 9.8;


            imu_msg.angular_velocity.x = static_cast<double>(angularVelocity[0]) / 32768.0 * 2000 * M_PI / 180;
            imu_msg.angular_velocity.y = static_cast<double>(angularVelocity[1]) / 32768.0 * 2000 * M_PI / 180;
            imu_msg.angular_velocity.z = static_cast<double>(angularVelocity[2]) / 32768.0 * 2000 * M_PI / 180;

            double roll = static_cast<double>(angle_degree[0]) / 32768.0 * M_PI;
            double pitch = static_cast<double>(angle_degree[1]) / 32768.0 * M_PI;
            double yaw = static_cast<double>(angle_degree[2]) / 32768.0 * M_PI;

            quat->setRPY(roll, pitch, yaw);

            imu_msg.orientation.x=quat->getX();
            imu_msg.orientation.y=quat->getY();
            imu_msg.orientation.z=quat->getZ();
            imu_msg.orientation.w=quat->getW();

            imu_msg.header.stamp=ros::Time::now();
        }
        else if (gps_data_ready.load())
        {
            gps_msg.longitude= static_cast<int>((lon_lat[0])/10000000.0)+((((lon_lat[0])%10000000)/100000.0)/60.0);

            gps_msg.latitude = static_cast<int>(((lon_lat[1])/10000000.0))+((((lon_lat[1])%10000000)/100000.0)/60.0);

            gps_msg.altitude = static_cast<double>(gps_data[0]) / 10.0;

            gps_msg.header.stamp=ros::Time::now();
        }
    }
}

/*
*描述：校验和函数
*作用：对每一组数据进行和校验
*参数：整组数据
*输出：数据校验是否正确
*/
bool ImuGpsReceiveSerial::checkSum(const std::vector<uint8_t> &data_buff)
{
    uint8_t sum = 0;
    for (size_t i = 0; i < data_buff.size() - 1; i++)
    {
        sum += data_buff[i];
    }
    return sum == data_buff[data_buff.size() - 1];
}

/*
*描述：16进制转换为short函数
*作用：将读取的16进制数转换为int16(short)类型，该类型为16位数
*参数：每一组数据的所有数据位
*输出：转换好的16位数，用于后续double类型处理
*/
std::vector<int16_t> ImuGpsReceiveSerial::hexToShort(const std::vector<uint8_t> &hex_data){
    std::vector<int16_t> short_data(4, 0);
    for (size_t i = 0; i < hex_data.size(); i += 2)
    {
        short high = static_cast<int16_t>(hex_data[i + 1]);
        short low = static_cast<int16_t>(hex_data[i]);
        short_data[i/2] = static_cast<int16_t>((high << 8) | low);
    }
    return short_data;
}

/*
*描述：16进制数转换为long函数
*作用：将读取的16进制数转换为int32(long)类型，该类型为32位数
*参数：每一组数据的所以数据位
*输出：转换好的32位数，用于后续double类型处理
*/
std::vector<int32_t> ImuGpsReceiveSerial::hexToLong(const std::vector<uint8_t> &hex_data){
    std::vector<int32_t> long_data(2, 0);
    for (size_t i = 0; i < hex_data.size(); i += 4)
    {
        long high_high  = static_cast<int32_t>(hex_data[i + 3]);
        long high_low   = static_cast<int32_t>(hex_data[i + 2]);
        long low_high   = static_cast<int32_t>(hex_data[i + 1]);
        long low_low    = static_cast<int32_t>(hex_data[i]);
        long_data[i/4]  = static_cast<int32_t>((high_high<<24) | (high_low<<16) | (low_high<<8) | low_low);
    }
    return long_data;
}
