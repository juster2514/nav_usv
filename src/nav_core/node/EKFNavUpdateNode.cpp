#include "kf_gins/EKFImuGnss.hpp"
#include "kf_gins/InsMech.hpp"
#include "kf_gins/earth.hpp"
#include "kf_gins/Rotation.hpp"

#include <ros/ros.h>
#include <yaml-cpp/yaml.h>

void loadConfig(YAML::Node &config,EKFOption &options);

int main(int argc, char** argv) {
  ros::init(argc, argv, "ekf_nav_update_node");
  std::string kf_config = "/home/ljj/Nav_USV_ws/src/nav_core/params/kf-gins.yaml";
  YAML::Node yamlConfig = YAML::LoadFile(kf_config);
  EKFOption options;
  loadConfig(yamlConfig,options);
  EKFImuGnss imugnss(options);
  ros::spin();
  return 0;
}

void loadConfig(YAML::Node &config,EKFOption &options){

    std::vector<double> vec1, vec2, vec3, vec4, vec5, vec6;
    vec1 = config["initpos"].as<std::vector<double>>();
    vec2 = config["initvel"].as<std::vector<double>>();
    vec3 = config["initatt"].as<std::vector<double>>();

    for (int i = 0; i < 3; i++) {
        options.initstate.pos[i]   = vec1[i]*D2R;
        options.initstate.vel[i]   = vec2[i];
        options.initstate.euler[i] = vec3[i]*D2R;
    }
    options.initstate.pos[2] *= R2D;

    // 读取IMU误差初始值(零偏和比例因子)
    // load initial imu error (bias and scale factor)
    vec1 = config["initgyrbias"].as<std::vector<double>>();
    vec2 = config["initaccbias"].as<std::vector<double>>();
    vec3 = config["initgyrscale"].as<std::vector<double>>();
    vec4 = config["initaccscale"].as<std::vector<double>>();
    for (int i = 0; i < 3; i++) {
        options.initstate.imuerror.gyrbias[i]  = vec1[i] * D2R / 3600.0;
        options.initstate.imuerror.accbias[i]  = vec2[i] * 1e-5;
        options.initstate.imuerror.gyrscale[i] = vec3[i] * 1e-6;
        options.initstate.imuerror.accscale[i] = vec4[i] * 1e-6;
    }

    // 读取初始位置、速度、姿态(欧拉角)的标准差
    // load initial position std, velocity std and attitude(euler angle) std

    vec1 = config["initposstd"].as<std::vector<double>>();
    vec2 = config["initvelstd"].as<std::vector<double>>();
    vec3 = config["initattstd"].as<std::vector<double>>();
    for (int i = 0; i < 3; i++) {
        options.initstate_std.pos[i]   = vec1[i];
        options.initstate_std.vel[i]   = vec2[i];
        options.initstate_std.euler[i] = vec3[i] * D2R;
    }

    // 读取IMU噪声参数
    // load imu noise parameters

    vec1 = config["imunoise"]["arw"].as<std::vector<double>>();
    vec2 = config["imunoise"]["vrw"].as<std::vector<double>>();
    vec3 = config["imunoise"]["gbstd"].as<std::vector<double>>();
    vec4 = config["imunoise"]["abstd"].as<std::vector<double>>();
    vec5 = config["imunoise"]["gsstd"].as<std::vector<double>>();
    vec6 = config["imunoise"]["asstd"].as<std::vector<double>>();

    options.imunoise.corr_time = config["imunoise"]["corrtime"].as<double>();
    for (int i = 0; i < 3; i++) {
        options.imunoise.gyr_arw[i]      = vec1[i];
        options.imunoise.acc_vrw[i]      = vec2[i];
        options.imunoise.gyrbias_std[i]  = vec3[i];
        options.imunoise.accbias_std[i]  = vec4[i];
        options.imunoise.gyrscale_std[i] = vec5[i];
        options.imunoise.accscale_std[i] = vec6[i];
    }

    // 读取IMU误差初始标准差,如果配置文件中没有设置，则采用IMU噪声参数中的零偏和比例因子的标准差
    // Load initial imu bias and scale std, set to bias and scale instability std if load failed
    vec1 = {options.imunoise.gyrbias_std.x(), options.imunoise.gyrbias_std.y(), options.imunoise.gyrbias_std.z()};
    vec2 = {options.imunoise.accbias_std.x(), options.imunoise.accbias_std.y(), options.imunoise.accbias_std.z()};
    vec3 = {options.imunoise.gyrscale_std.x(), options.imunoise.gyrscale_std.y(),options.imunoise.gyrscale_std.z()};
    vec4 = {options.imunoise.accscale_std.x(), options.imunoise.accscale_std.y(),options.imunoise.accscale_std.z()};

    // IMU初始误差转换为标准单位
    // convert initial imu errors' units to standard units
    for (int i = 0; i < 3; i++) {
        options.initstate_std.imuerror.gyrbias[i]  = vec1[i] * D2R / 3600.0;
        options.initstate_std.imuerror.accbias[i]  = vec2[i] * 1e-5;
        options.initstate_std.imuerror.gyrscale[i] = vec3[i] * 1e-6;
        options.initstate_std.imuerror.accscale[i] = vec4[i] * 1e-6;
    }

    // IMU噪声参数转换为标准单位
    // convert imu noise parameters' units to standard units
    options.imunoise.gyr_arw *= (D2R / 60.0);
    options.imunoise.acc_vrw /= 60.0;
    options.imunoise.gyrbias_std *= (D2R / 3600.0);
    options.imunoise.accbias_std *= 1e-5;
    options.imunoise.gyrscale_std *= 1e-6;
    options.imunoise.accscale_std *= 1e-6;
    options.imunoise.corr_time *= 3600;

    // GNSS天线杆臂, GNSS天线相位中心在IMU坐标系下位置
    // gnss antenna leverarm, position of GNSS antenna phase center in IMU frame
    vec1 = config["antlever"].as<std::vector<double>>();
    options.antlever = Eigen::Vector3d(vec1.data());
}