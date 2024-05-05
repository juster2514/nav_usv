#include "kf_gins/EKFImuGnss.hpp"
#include "kf_gins/InsMech.hpp"
#include "kf_gins/earth.hpp"
#include "nav_core/pos_vel_att_msg.h"



EKFImuGnss::EKFImuGnss(EKFOption &options):nh_("~"){

    this->options_ = options;
    kf_.Cov_.resize(RANK,RANK);
    kf_.Qc_.resize(NOISERANK,NOISERANK);
    kf_.dx_.resize(RANK,1);
    kf_.Cov_.setZero();
    kf_.Qc_.setZero();
    kf_.dx_.setZero();

    auto imunoise = options_.imunoise;
    kf_.Qc_.block(ARW_ID,ARW_ID,3,3) = imunoise.gyr_arw.cwiseProduct(imunoise.gyr_arw).asDiagonal();
    kf_.Qc_.block(VRW_ID,VRW_ID,3,3) = imunoise.acc_vrw.cwiseProduct(imunoise.acc_vrw).asDiagonal();
    kf_.Qc_.block(BGSTD_ID,BGSTD_ID,3,3) = 2 / imunoise.corr_time * imunoise.gyrbias_std.cwiseProduct(imunoise.gyrbias_std).asDiagonal();
    kf_.Qc_.block(BASTD_ID,BASTD_ID,3,3) = 2 / imunoise.corr_time * imunoise.accbias_std.cwiseProduct(imunoise.accbias_std).asDiagonal();
    kf_.Qc_.block(SGSTD_ID,SGSTD_ID,3,3) = 2 / imunoise.corr_time * imunoise.gyrscale_std.cwiseProduct(imunoise.gyrscale_std).asDiagonal();
    kf_.Qc_.block(SASTD_ID,SASTD_ID,3,3) = 2 / imunoise.corr_time * imunoise.accscale_std.cwiseProduct(imunoise.accscale_std).asDiagonal();

    llh_pre.reserve(3);
    xyz_pre.reserve(3);
    llh_pre[0]=options_.initstate.pos[0];
    llh_pre[1]=options_.initstate.pos[1];
    llh_pre[2]=options_.initstate.pos[2];

    inittime_flag = false;
    
    InitEKFImuGnss(options_.initstate, options_.initstate_std);

    pub_path_ =  nh_.advertise<nav_msgs::Path>("nav_path_show",100);
    run_status_ = nh_.advertise<nav_core::pos_vel_att_msg>("run_status_data",100);

    sub_imu_data_ = nh_.subscribe<sensor_msgs::Imu>("/imu_gps_node/imu_data",100,&EKFImuGnss::ImuCallback,this);
    sub_gps_data_ = nh_.subscribe<sensor_msgs::NavSatFix>("/imu_gps_node/gps_data",100,&EKFImuGnss::GpsCallback,this);
}

void EKFImuGnss::InitEKFImuGnss(const NavState &initstate, const NavState &initstate_std){
    pvacurrent_.pos       = initstate.pos;
    pvacurrent_.vel       = initstate.vel;
    pvacurrent_.att.Euler = initstate.euler;
    printf("init__pva_pos:%f\t%f\t%f\t\n",pvacurrent_.pos[0],pvacurrent_.pos[1],pvacurrent_.pos[2]);
    printf("init__pva_att:%f\t%f\t%f\t\n",pvacurrent_.att.Euler[0],pvacurrent_.att.Euler[1],pvacurrent_.att.Euler[2]);
    pvacurrent_.att.Cbn   = Rotation::euler2matrix(pvacurrent_.att.Euler);
    pvacurrent_.att.Qbn   = Rotation::euler2quaternion(pvacurrent_.att.Euler);

    imuerror_ = initstate.imuerror;

    pvaprediction_ = pvacurrent_;

    ImuError imuerror_std = initstate_std.imuerror;
    kf_.Cov_.block(P_ID, P_ID, 3, 3)     = initstate_std.pos.cwiseProduct(initstate_std.pos).asDiagonal();
    kf_.Cov_.block(V_ID, V_ID, 3, 3)     = initstate_std.vel.cwiseProduct(initstate_std.vel).asDiagonal();
    kf_.Cov_.block(ATT_ID, ATT_ID, 3, 3) = initstate_std.euler.cwiseProduct(initstate_std.euler).asDiagonal();
    kf_.Cov_.block(BG_ID, BG_ID, 3, 3)   = imuerror_std.gyrbias.cwiseProduct(imuerror_std.gyrbias).asDiagonal();
    kf_.Cov_.block(BA_ID, BA_ID, 3, 3)   = imuerror_std.accbias.cwiseProduct(imuerror_std.accbias).asDiagonal();
    kf_.Cov_.block(SG_ID, SG_ID, 3, 3)   = imuerror_std.gyrscale.cwiseProduct(imuerror_std.gyrscale).asDiagonal();
    kf_.Cov_.block(SA_ID, SA_ID, 3, 3)   = imuerror_std.accscale.cwiseProduct(imuerror_std.accscale).asDiagonal();

    LLA2XYZ(llh_pre,xyz_pre);

}

void EKFImuGnss::ImuCallback(const sensor_msgs::ImuConstPtr &imu_msg){
    IMU imu_temp;
    double gravity = Earth::gravity(pvacurrent_.pos);
    if (inittime_flag)
    {
        imu_temp.time = imu_msg->header.stamp.toSec();
        imu_temp.dt = imu_temp.time-timestamp_;
        timestamp_ = imu_temp.time;

        temp_cur.dacc[0] = imu_msg->angular_velocity.y;
        temp_cur.dacc[1] = imu_msg->angular_velocity.x;
        temp_cur.dacc[2] = imu_msg->angular_velocity.z;

        temp_cur.dvcc[0] = imu_msg->linear_acceleration.y;
        temp_cur.dvcc[1] = imu_msg->linear_acceleration.x;
        temp_cur.dvcc[2] = imu_msg->linear_acceleration.z;


        imu_temp.dangle[0] = (temp_pre.dacc[0] + temp_cur.dacc[0]) * imu_temp.dt *0.5;
        imu_temp.dangle[1] = (temp_pre.dacc[1] + temp_cur.dacc[1]) * imu_temp.dt *0.5;
        imu_temp.dangle[2] = -(temp_pre.dacc[2] + temp_pre.dacc[2]) * imu_temp.dt *0.5;

        imu_temp.dvel[0] = (temp_pre.dvcc[0] + temp_cur.dvcc[0]) * imu_temp.dt *0.5;
        imu_temp.dvel[1] = (temp_pre.dvcc[1] + temp_cur.dvcc[1]) * imu_temp.dt *0.5;  
        imu_temp.dvel[2] = -(temp_pre.dvcc[2] + temp_cur.dvcc[2]) * imu_temp.dt *0.5; 

        temp_pre=temp_cur;

        printf("dvel_x:%f\n",imu_temp.dvel[0]);
        printf("dvel_y:%f\n",imu_temp.dvel[1]);
        printf("dvel_z:%f\n",imu_temp.dvel[2]);


        addImuData(imu_temp,true);

        newImuProcess();
    }
    else if(!inittime_flag)
    {
        timestamp_=imu_msg->header.stamp.toSec();
        imu_temp.time = imu_msg->header.stamp.toSec();

        temp_pre.dacc[0] = 0;
        temp_pre.dacc[1] = 0;
        temp_pre.dacc[2] = 0;

        temp_pre.dvcc[0] = 0;
        temp_pre.dvcc[1] = 0;
        temp_pre.dvcc[2] = 0;

        imu_temp.dangle[0] = 0;
        imu_temp.dangle[1] = 0;
        imu_temp.dangle[2] = 0;

        imu_temp.dvel[0] = 0;
        imu_temp.dvel[1] = 0; 
        imu_temp.dvel[2] = 0; 

        imuprediction_ = imu_temp;
        imucurrent_ = imu_temp;
        inittime_flag=true;
    }
}

void EKFImuGnss::GpsCallback(const sensor_msgs::NavSatFixConstPtr &gps_msg){

    GNSS gnss_temp;
    gnss_temp.time = gps_msg->header.stamp.toSec();

    gnss_temp.llh[0] = gps_msg->latitude*D2R;
    gnss_temp.llh[1] = gps_msg->longitude*D2R;
    gnss_temp.llh[2] = gps_msg->altitude;

    gnss_temp.std << 0.0,0.0,0.0;

    addGnssData(gnss_temp);
}

void EKFImuGnss::addImuData(const IMU &imu, bool compensate = false){
    imuprediction_ = imucurrent_;
    imucurrent_ = imu;

    if (compensate) {
        ImuCompensate(imuprediction_);
    }
}

void EKFImuGnss::addGnssData(const GNSS &gnss){
    gnssdata_ = gnss;
}

void EKFImuGnss::ImuCompensate(IMU &imu){
    imu.dangle -=imuerror_.gyrbias*imu.dt;
    imu.dvel   -=imuerror_.accbias*imu.dt;

    Eigen::Vector3d gyrscale , accscale;
    gyrscale   = Eigen::Vector3d::Ones() + imuerror_.gyrscale;
    accscale   = Eigen::Vector3d::Ones() + imuerror_.accscale;
    imu.dangle = imu.dangle.cwiseProduct(gyrscale.cwiseInverse());
    imu.dvel   = imu.dvel.cwiseProduct(accscale.cwiseInverse());

}

void EKFImuGnss::newImuProcess(){
    double updatetime = gnssdata_.time;

    int res = isToUpdate(imuprediction_.time,imucurrent_.time,updatetime);

    if (res == 0)
    {
        ImuPropagation(imuprediction_,imucurrent_);
    }else if (res == 1)
    {
        GnssUpdate(gnssdata_);
        StateFeedback();

        pvaprediction_ = pvacurrent_;
        ImuPropagation(imuprediction_,imucurrent_);
    }else if (res == 2)
    {
        ImuPropagation(imuprediction_,imucurrent_);
        GnssUpdate(gnssdata_);
        StateFeedback();
    }else{
        IMU midimu;
        
        imuInterpolate(imuprediction_,imucurrent_,updatetime,midimu);

        ImuPropagation(imuprediction_,midimu);

        GnssUpdate(gnssdata_);

        StateFeedback();

        pvaprediction_ = pvacurrent_;

        ImuPropagation(midimu,imucurrent_);
    }

    pvaprediction_ = pvacurrent_;
    imuprediction_ = imucurrent_;

    PosAtt2Path();
    
}

int EKFImuGnss::isToUpdate(double imutime1, double imutime2, double updatetime) const{

    if (abs(imutime1 - updatetime) < TIME_ALIGN_ERR) {
        // 更新时间靠近imutime1
        return 1;
    } else if (abs(imutime2 - updatetime) <= TIME_ALIGN_ERR) {
        // 更新时间靠近imutime2
        return 2;
    } else if (imutime1 < updatetime && updatetime < imutime2) {
        // 更新时间在imutime1和imutime2之间, 但不靠近任何一个
        return 3;
    } else {
        // 更新时间不在imutimt1和imutime2之间，且不靠近任何一个
        return 0;
    }

}

void EKFImuGnss::ImuPropagation(IMU &imuprediction, IMU &imucurrent){

    ImuCompensate(imucurrent);

    InsMech::insMech(pvaprediction_,pvacurrent_,imuprediction,imucurrent);

    Eigen::MatrixXd Phi,F,Qd,G;

    Phi.resizeLike(kf_.Cov_);
    F.resizeLike(kf_.Cov_);
    Qd.resizeLike(kf_.Cov_);
    G.resize(RANK, NOISERANK);
    Phi.setIdentity();
    F.setZero();
    Qd.setZero();
    G.setZero();

    Eigen::Vector2d rmrn;
    Eigen::Vector3d wie_n, wen_n;
    Eigen::Matrix3d temp;
    Eigen::Vector3d accel, omega;
    double rmh, rnh;
    double gravity;

    rmrn    = Earth::meridianPrimeVerticalRadius(pvaprediction_.pos[0]);
    rmh   = rmrn[0] + pvaprediction_.pos[2];
    rnh   = rmrn[1] + pvaprediction_.pos[2];
    gravity = Earth::gravity(pvaprediction_.pos);
    accel = imucurrent.dvel / imucurrent.dt;
    omega = imucurrent.dangle / imucurrent.dt;

    // 位置误差
    temp.setZero();
    temp(0, 0)                = -pvaprediction_.vel[2] / rmh;
    temp(0, 2)                = pvaprediction_.vel[0] / rmh;
    temp(1, 0)                = pvaprediction_.vel[1] * tan(pvaprediction_.pos[0]) / rnh;
    temp(1, 1)                = -(pvaprediction_.vel[2] + pvaprediction_.vel[0] * tan(pvaprediction_.pos[0])) / rnh;
    temp(1, 2)                = pvaprediction_.vel[1] / rnh;
    F.block(P_ID,P_ID,3,3)    = temp;
    F.block(P_ID,V_ID,3,3)    = Eigen::Matrix3d::Identity();

    // 速度误差
    temp.setZero();
    temp(0, 0) = -2 * pvaprediction_.vel[1] * WGS84_WIE * cos(pvaprediction_.pos[0]) / rmh -
                 pow(pvaprediction_.vel[1], 2) / rmh / rnh / pow(cos(pvaprediction_.pos[0]), 2);
    temp(0, 2) = pvaprediction_.vel[0] * pvaprediction_.vel[2] / rmh / rmh - pow(pvaprediction_.vel[1], 2) * tan(pvaprediction_.pos[0]) / rnh / rnh;
    temp(1, 0) = 2 * WGS84_WIE * (pvaprediction_.vel[0] * cos(pvaprediction_.pos[0]) - pvaprediction_.vel[2] * sin(pvaprediction_.pos[0])) / rmh +
                 pvaprediction_.vel[0] * pvaprediction_.vel[1] / rmh / rnh / pow(cos(pvaprediction_.pos[0]), 2);
    temp(1, 2) = (pvaprediction_.vel[1] * pvaprediction_.vel[2] + pvaprediction_.vel[0] * pvaprediction_.vel[1] * tan(pvaprediction_.pos[0])) / rnh / rnh;
    temp(2, 0) = 2 * WGS84_WIE * pvaprediction_.vel[1] * sin(pvaprediction_.pos[0]) / rmh;
    temp(2, 2) = -pow(pvaprediction_.vel[1], 2) / rnh / rnh - pow(pvaprediction_.vel[0], 2) / rmh / rmh +
                 2 * gravity / (sqrt(rmrn[0] * rmrn[1]) + pvaprediction_.pos[2]);
    F.block(V_ID, P_ID, 3, 3) = temp;
    temp.setZero();
    temp(0, 0)                  = pvaprediction_.vel[2] / rmh;
    temp(0, 1)                  = -2 * (WGS84_WIE * sin(pvaprediction_.pos[0]) + pvaprediction_.vel[1] * tan(pvaprediction_.pos[0]) / rnh);
    temp(0, 2)                  = pvaprediction_.vel[0] / rmh;
    temp(1, 0)                  = 2 * WGS84_WIE * sin(pvaprediction_.pos[0]) + pvaprediction_.vel[1] * tan(pvaprediction_.pos[0]) / rnh;
    temp(1, 1)                  = (pvaprediction_.vel[2] + pvaprediction_.vel[0] * tan(pvaprediction_.pos[0])) / rnh;
    temp(1, 2)                  = 2 * WGS84_WIE * cos(pvaprediction_.pos[0]) + pvaprediction_.vel[1] / rnh;
    temp(2, 0)                  = -2 * pvaprediction_.vel[0] / rmh;
    temp(2, 1)                  = -2 * (WGS84_WIE * cos(pvaprediction_.pos(0)) + pvaprediction_.vel[1] / rnh);
    F.block(V_ID, V_ID, 3, 3)   = temp;
    F.block(V_ID,ATT_ID,3,3)  = Rotation::skewSymmetric(pvaprediction_.att.Cbn*accel);
    F.block(V_ID,BA_ID,3,3)   = pvaprediction_.att.Cbn;
    F.block(V_ID,SA_ID,3,3)   = pvaprediction_.att.Cbn * (accel.asDiagonal());

    // 姿态误差
    temp.setZero();
    temp(0, 0) = -WGS84_WIE * sin(pvaprediction_.pos[0]) / rmh;
    temp(0, 2) = pvaprediction_.vel[1] / rnh / rnh;
    temp(1, 2) = -pvaprediction_.vel[0] / rmh / rmh;
    temp(2, 0) = -WGS84_WIE * cos(pvaprediction_.pos[0]) / rmh - pvaprediction_.vel[1] / rmh / rnh / pow(cos(pvaprediction_.pos[0]), 2);
    temp(2, 2) = -pvaprediction_.vel[1] * tan(pvaprediction_.pos[0]) / rnh / rnh;
    F.block(ATT_ID, P_ID, 3, 3) = temp;
    temp.setZero();
    temp(0, 1)                    = 1 / rnh;
    temp(1, 0)                    = -1 / rmh;
    temp(2, 1)                    = -tan(pvaprediction_.pos[0]) / rnh;
    F.block(ATT_ID, V_ID, 3, 3)   = temp;
    F.block(ATT_ID,BG_ID,3,3) = -pvaprediction_.att.Cbn;
    F.block(ATT_ID,SG_ID,3,3) = -pvaprediction_.att.Cbn * (omega.asDiagonal());

    F.block(BG_ID, BG_ID, 3, 3) = -1 / options_.imunoise.corr_time * Eigen::Matrix3d::Identity();
    F.block(BA_ID, BA_ID, 3, 3) = -1 / options_.imunoise.corr_time * Eigen::Matrix3d::Identity();
    F.block(SG_ID, SG_ID, 3, 3) = -1 / options_.imunoise.corr_time * Eigen::Matrix3d::Identity();
    F.block(SA_ID, SA_ID, 3, 3) = -1 / options_.imunoise.corr_time * Eigen::Matrix3d::Identity();

    G.block(V_ID,VRW_ID,3,3)    = pvaprediction_.att.Cbn;
    G.block(ATT_ID,ARW_ID,3,3)  = pvaprediction_.att.Cbn;
    G.block(BG_ID,BGSTD_ID,3,3) = Eigen::Matrix3d::Identity();
    G.block(BA_ID,BASTD_ID,3,3) = Eigen::Matrix3d::Identity();
    G.block(SG_ID,SGSTD_ID,3,3) = Eigen::Matrix3d::Identity();
    G.block(SA_ID,SASTD_ID,3,3) = Eigen::Matrix3d::Identity();

    Phi.setIdentity();
    Phi = Phi + F *imucurrent.dt;

    Qd = G * kf_.Qc_ * G.transpose() * imucurrent.dt;
    Qd = (Phi * Qd * Phi.transpose() + Qd ) / 2;

    EKfPredict(Phi,Qd);

}

void EKFImuGnss::GnssUpdate(GNSS &gnssdate){
    Eigen::Vector3d antenna_pos;
    Eigen::Matrix3d Dr, Dr_inv;

    Dr_inv      = Earth::DRi(pvacurrent_.pos);
    Dr          = Earth::DR(pvacurrent_.pos);

    antenna_pos = pvacurrent_.pos + Dr_inv * pvacurrent_.att.Cbn * options_.antlever ;

    Eigen::MatrixXd dz;
    dz = Dr * (antenna_pos - gnssdate.llh);

    Eigen::MatrixXd H_gnsspos;

    H_gnsspos.resize(3,kf_.Cov_.rows());
    H_gnsspos.setZero();
    H_gnsspos.block(0,P_ID,3,3)   = Eigen::Matrix3d::Identity();
    H_gnsspos.block(0,ATT_ID,3,3) = Rotation::skewSymmetric(pvacurrent_.att.Cbn * options_.antlever); 

    Eigen::MatrixXd R_gnsspos;
    R_gnsspos = gnssdate.std.cwiseProduct(gnssdate.std).asDiagonal();

    EKFUpdate(dz,H_gnsspos,R_gnsspos);
}

void EKFImuGnss::EKfPredict(Eigen::MatrixXd &Phi, Eigen::MatrixXd &Qd){

    assert(Phi.rows() == kf_.Cov_.rows());
    assert(Qd.rows() == kf_.Cov_.rows());

    kf_.Cov_ = Phi * kf_.Cov_ * Phi.transpose() + Qd;
    kf_.dx_  = Phi * kf_.dx_;
}

void EKFImuGnss::EKFUpdate(Eigen::MatrixXd &dz, Eigen::MatrixXd &H, Eigen::MatrixXd &R){

    assert(H.cols() == kf_.Cov_.rows());
    assert(dz.rows() == H.rows());
    assert(dz.rows() == R.rows());
    assert(dz.cols() == 1);

    auto temp         = H * kf_.Cov_ * H.transpose() + R;
    Eigen::MatrixXd K = kf_.Cov_ * H.transpose() * temp.inverse();

    Eigen::MatrixXd I;
    I.resizeLike(kf_.Cov_);
    I.setIdentity();
    I = I - K * H;

    kf_.dx_  = kf_.dx_ + K * (dz - H * kf_.dx_);
    kf_.Cov_ = I * kf_.Cov_ * I.transpose() + K * R * K.transpose();

}

void EKFImuGnss::PosAtt2Path(){
    std::vector<double> llh(3),xyz(3);

    llh[0] = pvacurrent_.pos[0]; //pos顺序纬度经度-lat lon height
    llh[1] = pvacurrent_.pos[1];
    llh[2] = pvacurrent_.pos[2];

    LLA2XYZ(llh,xyz);

    pos_msg.pose.position.x = xyz[0]-xyz_pre[0];
    pos_msg.pose.position.y = xyz[1]-xyz_pre[1];
    pos_msg.pose.position.z = xyz[2]-xyz_pre[2];

    run_statue_msg.pos.pose.position.x = xyz[0]-xyz_pre[0]; 
    run_statue_msg.pos.pose.position.y = xyz[1]-xyz_pre[1]; 
    run_statue_msg.pos.pose.position.z = xyz[2]-xyz_pre[2]; 

    pos_msg.pose.orientation.w = pvacurrent_.att.Qbn.w();
    pos_msg.pose.orientation.x = pvacurrent_.att.Qbn.x();
    pos_msg.pose.orientation.y = pvacurrent_.att.Qbn.y();
    pos_msg.pose.orientation.z = pvacurrent_.att.Qbn.z();

    run_statue_msg.pos.pose.orientation = pos_msg.pose.orientation;

    run_statue_msg.vel.x = pvacurrent_.vel[0];
    run_statue_msg.vel.y = pvacurrent_.vel[1];
    run_statue_msg.vel.z = pvacurrent_.vel[2];
    
    path_msg.poses.push_back(pos_msg);
    path_msg.header.stamp = ros::Time::now();
    path_msg.header.frame_id = "map";
    run_statue_msg.header.stamp = ros::Time::now();
    run_statue_msg.header.frame_id = "run_statue";

    pub_path_.publish(path_msg);
    run_status_.publish(run_statue_msg);
}

void EKFImuGnss::LLA2XYZ(std::vector<double> &llh,std::vector<double> &xyz)
{
    double lat = llh[0];      
    double lon = llh[1];
    double hei = llh[2];

    double a = WGS84_RA;     
    double b = WGS84_RB;    

    double N = a/(sqrt(1-((a * a - b * b) / (a * a)) * sin(lat) * sin(lat)));

    xyz[0] =(N + hei ) * cos(lat) * cos(lon);
    xyz[1] =(N + hei ) * cos(lat) * sin(lon);
    xyz[2] =((b * b * N)/ (a * a) + hei) * sin(lat) ;
}

NavState EKFImuGnss::GetNavState(){
    NavState state;

    state.pos      = pvacurrent_.pos;
    state.vel      = pvacurrent_.vel;
    state.euler    = pvacurrent_.att.Euler;
    state.imuerror = imuerror_;

    return state;
}

void EKFImuGnss::StateFeedback(){
    Eigen::Vector3d temp;

    Eigen::Vector3d deltar_r = kf_.dx_.block(P_ID,0,3,1);
    Eigen::Matrix3d Dr_inv = Earth::DRi(pvacurrent_.pos);
    pvacurrent_.pos -=Dr_inv * deltar_r;

    temp = kf_.dx_.block(V_ID,0,3,1);
    pvacurrent_.vel -=temp;

    temp = kf_.dx_.block(ATT_ID,0,3,1);
    Eigen::Quaterniond Qpn = Rotation::rotvec2quaternion(temp);
    pvacurrent_.att.Qbn = Qpn * pvacurrent_.att.Qbn;
    pvacurrent_.att.Cbn = Rotation::quaternion2matrix(pvacurrent_.att.Qbn);
    pvacurrent_.att.Euler = Rotation::matrix2euler(pvacurrent_.att.Cbn); 

    temp = kf_.dx_.block(BG_ID, 0, 3, 1);
    imuerror_.gyrbias += temp;
    temp = kf_.dx_.block(BA_ID, 0, 3, 1);
    imuerror_.accbias += temp;

    temp = kf_.dx_.block(SG_ID,0,3,1);
    imuerror_.gyrscale +=temp;
    temp = kf_.dx_.block(SA_ID,0,3,1);
    imuerror_.accscale += temp;

    kf_.dx_.setZero();

}