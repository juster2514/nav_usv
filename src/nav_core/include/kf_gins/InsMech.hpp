#ifndef INSMECH_HPP
#define INSMECH_HPP

#include "kf_gins/EKF_types.hpp"
#include "kf_gins/earth.hpp"
#include "kf_gins/Rotation.hpp"

class InsMech{
 public:
    static void insMech(const PosVelAtt &pvapre,PosVelAtt &pvacur,const IMU &imupre,const IMU &imucur){
        velUpdate(pvapre,pvacur,imupre,imucur);
        posUpdate(pvapre,pvacur,imupre,imucur);
        attUpdate(pvapre,pvacur,imupre,imucur);
    };
 private:
    static void velUpdate(const PosVelAtt &pvapre, PosVelAtt &pvacur, const IMU &imupre, const IMU &imucur){
        Eigen::Vector3d temp1,temp2,temp3;
        Eigen::Vector3d d_vfb,d_vfn,g;

        double gravity = Earth::gravity(pvapre.pos);
        g << 0,0,gravity;

        temp1 = imucur.dangle.cross(imucur.dvel) / 2;
        temp2 = imucur.dangle.cross(imucur.dvel) / 12;
        temp3 = imucur.dvel.cross(imucur.dangle) / 12;

        d_vfb = imucur.dvel + temp1 + temp2 + temp3;

        d_vfn = pvapre.att.Cbn * d_vfb;

        pvacur.vel = pvapre.vel + d_vfn + g*imucur.dt;
    };
    static void posUpdate(const PosVelAtt &pvapre, PosVelAtt &pvacur, const IMU &imupre, const IMU &imucur){
        Eigen::Vector3d temp1, temp2, midvel, midpos;
        Eigen::Vector2d rmn = Earth::meridianPrimeVerticalRadius(pvapre.pos[0]);

        midvel = (pvacur.vel+pvapre.vel)/2;
        midpos = pvapre.pos + Earth::DRi(pvapre.pos) * midvel *imucur.dt/2;

        pvacur.pos[2] = pvapre.pos[2] - midvel[2]*imucur.dt;
        pvacur.pos[0] = pvapre.pos[0] + (midvel[0]/(rmn[0]+midpos[2]))*imucur.dt;
        pvacur.pos[1] = pvapre.pos[1] + (midvel[1]/((rmn[1]+midpos[2])*cos(midpos[0])))*imucur.dt;

    };
    static void attUpdate(const PosVelAtt &pvapre, PosVelAtt &pvacur, const IMU &imupre, const IMU &imucur){
        Eigen::Vector3d temp1;
        Eigen::Quaterniond qbb;

        temp1 = imucur.dangle + imupre.dangle.cross(imucur.dangle) / 12;
        qbb = Rotation::rotvec2quaternion(temp1);

        pvacur.att.Qbn = pvapre.att.Qbn * qbb;
        pvacur.att.Cbn = Rotation::quaternion2matrix(pvacur.att.Qbn);
        pvacur.att.Euler = Rotation::matrix2euler(pvacur.att.Cbn);
    };
};

#endif