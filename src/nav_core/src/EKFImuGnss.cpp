#include "kf_gins/EKFImuGnss.hpp"
#include "kf_gins/InsMech.hpp"
#include "kf_gins/earth.hpp"



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

    InitEKFImuGnss(options_.initstate, options_.initstate_std);

    sub_imu_data_ = nh_.subscribe<sensor_msgs::Imu>("/imu_gps_node/imu_data",100,&EKFImuGnss::ImuCallback,this);
    sub_gps_data_ = nh_.subscribe<sensor_msgs::NavSatFix>("/imu_gps_node/gps_data",100,&EKFImuGnss::GpsCallback,this);
    pub_path_ =  nh_.advertise<nav_msgs::Path>("nav_path_show",100);
}

void EKFImuGnss::InitEKFImuGnss(const NavState &initstate, const NavState &initstate_std){
    pvacurrent_.pos       = initstate.pos;
    pvacurrent_.vel       = initstate.vel;
    pvacurrent_.att.Euler = initstate.euler;
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
    imu_temp.time = imu_msg->header.stamp.toSec();
    imu_temp.dt = imu_temp.time-timestamp_;
    timestamp_ = imu_temp.time;

    imu_temp.dangle[0] = imu_msg->angular_velocity.x * imu_temp.dt;
    imu_temp.dangle[1] = imu_msg->angular_velocity.y * imu_temp.dt;
    imu_temp.dangle[2] = imu_msg->angular_velocity.z * imu_temp.dt;

    imu_temp.dvel[0] = imu_msg->linear_acceleration.x *imu_temp.dt;
    imu_temp.dvel[1] = imu_msg->linear_acceleration.y *imu_temp.dt; 
    imu_temp.dvel[2] = imu_msg->linear_acceleration.z *imu_temp.dt; 

    addImuData(imu_temp,true);

    newImuProcess();

}

void EKFImuGnss::GpsCallback(const sensor_msgs::NavSatFixConstPtr &gps_msg){

    GNSS gnss_temp;
    gnss_temp.time = gps_msg->header.stamp.toSec();

    gnss_temp.llh[0] = gps_msg->latitude;
    gnss_temp.llh[1] = gps_msg->longitude;
    gnss_temp.llh[2] = gps_msg->altitude;

    addGnssData(gnss_temp);
}

void EKFImuGnss::addImuData(const IMU &imu, bool compensate = false){
    imuprediction_ = imucurrent_;
    imucurrent_ = imu;

    if (compensate) {
        ImuCompensate(imucurrent_);
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
    Phi.setIdentity();
    F.setZero();
    Qd.setZero();
    G.setZero();

    Eigen::Vector3d accel, omega;
    accel = imucurrent.dvel / imucurrent.dt;
    omega = imucurrent.dangle / imucurrent.dt;

    F.block(P_ID,V_ID,3,3)    = Eigen::Matrix3d::Identity();

    F.block(V_ID,ATT_ID,3,3)  = Rotation::skewSymmetric(pvaprediction_.att.Cbn*accel);
    F.block(V_ID,BA_ID,3,3)   = pvaprediction_.att.Cbn;
    F.block(V_ID,SA_ID,3,3)   = pvaprediction_.att.Cbn * (accel.asDiagonal());

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

    antenna_pos = pvacurrent_.pos + pvacurrent_.att.Cbn * options_.antlever ;

    Eigen::MatrixXd dz;
    dz = antenna_pos - gnssdate.llh;

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
    std::vector<double> llh,xyz;

    llh[0] = pvacurrent_.pos[0]; //pos顺序纬度经度-lat lon height
    llh[1] = pvacurrent_.pos[1];
    llh[2] = pvacurrent_.pos[2];

    LLA2XYZ(llh,xyz);

    pos_msg.pose.position.x = xyz[0]-xyz_pre[0];
    pos_msg.pose.position.y = xyz[1]-xyz_pre[1];
    pos_msg.pose.position.z = xyz[2]-xyz_pre[2];

    pos_msg.pose.orientation.w = pvacurrent_.att.Qbn.w();
    pos_msg.pose.orientation.x = pvacurrent_.att.Qbn.x();
    pos_msg.pose.orientation.y = pvacurrent_.att.Qbn.y();
    pos_msg.pose.orientation.z = pvacurrent_.att.Qbn.z();

    path_msg.poses.push_back(pos_msg);
    path_msg.header.stamp = ros::Time::now();
    path_msg.header.frame_id = "map";
    pub_path_.publish(path_msg);
}

void EKFImuGnss::LLA2XYZ(std::vector<double> &llh,std::vector<double> &xyz)
{
    double lat = llh[0] * D2R;      
    double lon = llh[1]* D2R;
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

    temp = kf_.dx_.block(SG_ID,0,3,1);
    imuerror_.gyrscale +=temp;
    temp = kf_.dx_.block(SA_ID,0,3,1);
    imuerror_.accscale += temp;

    kf_.dx_.setZero();

}