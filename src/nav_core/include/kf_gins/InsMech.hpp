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
        g << 0, 0, gravity;

        temp1 = imucur.dangle.cross(imucur.dvel) / 2.0;
        temp2 = imupre.dangle.cross(imucur.dvel) / 12.0;
        temp3 = imupre.dvel.cross(imucur.dangle) / 12.0;

        d_vfb = imucur.dvel + temp1 + temp2 +temp3;

        d_vfn = pvapre.att.Cbn * d_vfb;

        pvacur.vel = pvapre.vel + d_vfn + g*imucur.dt;
        // printf("dt:%f\n",imucur.dt);
        printf("temp1:%f\t%f\t%f\n",temp1[0],temp1[1],temp1[2]);
        printf("d_vfb:%f\t%f\t%f\n",d_vfb[0],d_vfb[1],d_vfb[2]);
        printf("d_vfn:%f\t%f\t%f\n",d_vfn[0],d_vfn[1],d_vfn[2]);
        printf("ins_vel_x:%f\n",pvacur.vel[0]);
        printf("ins_vel_y:%f\n",pvacur.vel[1]);
        printf("ins_vel_z:%f\n",pvacur.vel[2]);

    };
    static void posUpdate(const PosVelAtt &pvapre, PosVelAtt &pvacur, const IMU &imupre, const IMU &imucur){
        Eigen::Vector3d temp1, temp2, midvel, midpos;
        Eigen::Vector2d rmn = Earth::meridianPrimeVerticalRadius(pvapre.pos[0]);

        midvel = (pvacur.vel+pvapre.vel)/2.0;
        midpos = pvapre.pos + Earth::DRi(pvapre.pos) * midvel *imucur.dt/2.0;

        pvacur.pos[2] = pvapre.pos[2] - midvel[2]*imucur.dt;
        pvacur.pos[0] = pvapre.pos[0] + (midvel[0]/(2*(rmn[0]+midpos[2])))*imucur.dt;
        pvacur.pos[1] = pvapre.pos[1] + (midvel[1]/(2*((rmn[1]+midpos[2])*cos(midpos[0]))))*imucur.dt;

        printf("ins_pos_x:%f\n",pvacur.pos[0]*R2D);
        printf("ins_pos_y:%f\n",pvacur.pos[1]*R2D);
        printf("ins_pos_z:%f\n",pvacur.pos[2]);


    };
    static void attUpdate(const PosVelAtt &pvapre, PosVelAtt &pvacur, const IMU &imupre, const IMU &imucur){
        Eigen::Vector3d temp1;
        Eigen::Quaterniond qbb;

        temp1 = imucur.dangle + imupre.dangle.cross(imucur.dangle) / 12.0;
        qbb = Rotation::rotvec2quaternion(temp1);

        pvacur.att.Qbn = pvapre.att.Qbn * qbb;
        pvacur.att.Cbn = Rotation::quaternion2matrix(pvacur.att.Qbn);
        pvacur.att.Euler = Rotation::matrix2euler(pvacur.att.Cbn);

        printf("ins_Eluler_r:%f\n",pvacur.att.Euler[0]*R2D);
        printf("ins_ELuler_p:%f\n",pvacur.att.Euler[1]*R2D);
        printf("ins_Eluler_y:%f\n",pvacur.att.Euler[2]*R2D);
    };
};

#endif