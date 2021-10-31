// r2_quaternion.h
// convert from a quaternion to another representation of rotation

#ifndef R2_QUATERNION_H
#define R2_QUATERNION_H

#include <math.h>

#ifndef R2_QUATERNION_STRUCT
#define R2_QUATERNION_STRUCT
// Quaternion q = q.h + q.i i + q.j j + q.k k;
typedef struct r2_qf_t {
    float h;
    float i;
    float j;
    float k;
};
#endif // R2_QUATERNION_STRUCT

#ifndef R2_NAUTICAL_ANGLE_STRUCT
#define R2_NAUTICAL_ANGLE_STRUCT
// (Tait-Bryan z-y'-x'' intrinsic) nautical angles, or Cardan angles
typedef struct r2_nf_t {
    float roll;
    float pitch;
    float yaw;
};
#endif // R2_NAUTICAL_ANGLE_STRUCT

struct r2_qf_t r2_nf_to_qf(const struct r2_nf_t);
// TODO ^ this one should probably move to r2_nautical_angle.h

struct r2_nf_t r2_qf_to_nf(const struct r2_qf_t);

struct r2_qf_t r2_nautical_angle_to_quaternion(const struct r2_nf_t nf){
    return r2_nf_to_qf(nf);
}
// TODO ^ this one should probably move to r2_nautical_angle.h

struct r2_qf_t nautical_angle_to_quaternion(const float roll, const float pitch, const float yaw){
    struct r2_nf_t rpy = { roll, pitch, yaw };
    return r2_nf_to_qf(rpy);
}
// TODO ^ this one should probably move to r2_nautical_angle.h

struct r2_nf_t r2_quaternion_to_nautical_angle(const struct r2_qf_t qf){
    return r2_qf_to_nf(qf);
}

#endif // R2_QUATERNION_H

#ifndef R2_QUATERNION_I
#define R2_QUATERNION_I


struct r2_qf_t r2_nf_to_qf(const struct r2_nf_t n){
    float cy = cos(n.yaw * 0.5);
    float sy = sin(n.yaw * 0.5);
    float cp = cos(n.pitch * 0.5);
    float sp = sin(n.pitch * 0.5);
    float cr = cos(n.roll * 0.5);
    float sr = sin(n.roll * 0.5);

    struct r2_qf_t q = { 0 };
    q.h = cr * cp * cy + sr * sp * sy;
    q.i = sr * cp * cy - cr * sp * sy;
    q.j = cr * sp * cy + sr * cp * sy;
    q.k = cr * cp * sy - sr * sp * cy;

    return q;
}

struct r2_nf_t r2_qf_to_nf(const struct r2_qf_t q){
    float sr_cp = 2 * (q.h * q.i + q.j * q.k);
    float cr_cp = 1 - 2 * (q.i * q.i + q.j * q.j);
    float sp = 2 * (q.h * q.j - q.k * q.i);
    float sy_cp = 2 * (q.h * q.k + q.i * q.j);
    float cy_cp = 1 - 2 * (q.j * q.j + q.k * q.k);

    // TODO: limit sp to [-1,1]
    // just in case it fell outside due to finite precision

    struct r2_nf_t n = { 0 };
    n.roll = atan2(sr_cp, cr_cp);
    n.pitch = asin(sp);
    n.yaw = atan2(sy_cp, cy_cp);

    return n;
} // TODO: clearer implementation by composing rotations about axes with axis-angle representations

#endif // R2_QUATERNION_I
