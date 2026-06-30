#pragma once

#include <Arduino.h>
#include <DFRobot_BMI160.h>
#include <Log.h>

class Gyro {
    public :
        Gyro(uint8_t i2c_addr = 0x69);
        void begin();
        void loop();

        float getTiltX() const;
        float getTiltY() const;
        bool isTilted(float threshold = 15.0f) const;
    
    private :
        DFRobot_BMI160 *_gyro;
        uint8_t _addr;
        float _gx = 0, _gy = 0, _gz = 0;
        float _ax = 0, _ay = 0, _az = 0;
        const float _alpha = 0.1f;
};