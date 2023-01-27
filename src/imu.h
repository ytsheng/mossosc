#pragma once

#include <LSM6DS3.h>
#include <MahonyAHRS.h>

class Imu
{

private:
    LSM6DS3Class imu;
    Mahony ahrs;
    bool started;

public:
    Imu() :
        imu(Wire, 0x6B), started(false)
        {}

    bool begin()
    {
        ahrs.begin(104);
        started = started || (imu.begin() == 1);
        return started;
    }

    void tick() 
    {
        float ax, ay, az;
        float gx, gy, gz;

        if (!started) {
            return;
        }

        if (imu.accelerationAvailable() && imu.gyroscopeAvailable() ) {
            imu.readAcceleration(az, ay, az);
            imu.readGyroscope(gx, gy, gz);
            ahrs.updateIMU(gx, gy, gz, ax, ay, az);
        }
    }

    void getOrientation(float &roll, float &pitch, float &yaw) 
    {
        roll = ahrs.getRoll();
        pitch = ahrs.getPitch();
        yaw = ahrs.getYaw();
    }

    float getRoll() {
        return ahrs.getRoll();
    }

    float getPitch() {
        return ahrs.getPitch();
    }

    float getYaw() {
        return ahrs.getYaw();
    }

    // bool calibrate(int delayMillis=250, int calibrationMillis=250) {
    //     int calibrationCount = 0;

    //     delay(delayMillis);

    //     float sumX, sumY, sumZ;
    //     int startTime = millis();
    //     while (millis() < startTime + calibrationMillis) {
    //         if (read()) {
    //             sumX += gyroX;
    //             sumY += gyroY;
    //             sumZ += gyroZ;

    //             calibrationCount++;
    //         }
    //     }

    //     if (calibrationCount == 0) {
    //         return false;
    //     }

    //     gyroDriftX = sumX / calibrationCount;
    //     gyroDriftY = sumY / calibrationCount;
    //     gyroDriftZ = sumZ / calibrationCount;

    //     return true;
    // }

};