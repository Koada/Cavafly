#include "AHRS.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------
// Definitions

#define Kp 2.0f                  // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.005f                // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.001f             // half the sample period

//---------------------------------------------------------------------------------------------------
// Variable definitions

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;        		// quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;          // scaled integral error

//====================================================================================================
// Function
//====================================================================================================

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az) {

        float norm;
        float vx, vy, vz;
        float ex, ey, ez;

        // normalise the measurements
        norm = sqrt(ax*ax + ay*ay + az*az);
        ax = ax / norm;
        ay = ay / norm;
        az = az / norm;


        // estimated direction of gravity
        vx = 2*(q1*q3 - q0*q2);
        vy = 2*(q0*q1 + q2*q3);
        vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;


        // error is sum of cross product between reference direction of field and direction measured by sensor
        ex = (ay*vz - az*vy);
        ey = (az*vx - ax*vz);
        ez = (ax*vy - ay*vx);


        // integral error scaled integral gain
        exInt = exInt + ex*Ki;
        eyInt = eyInt + ey*Ki;
        ezInt = ezInt + ez*Ki;

        // adjusted gyroscope measurements
        gx = gx + Kp*ex + exInt;
        gy = gy + Kp*ey + eyInt;
        gz = gz + Kp*ez + ezInt;





        // integrate quaternion rate and normalise
        q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
        q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
        q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
        q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;



        // normalise quaternion
        norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
        q0 = q0 / norm;
        q1 = q1 / norm;
        q2 = q2 / norm;
        q3 = q3 / norm;
}

//====================================================================================================
// END OF CODE
