#include <Gyro.h>

Gyro::Gyro(uint8_t i2c_addr) : _addr(i2c_addr) {}

void Gyro::begin() {
    _gyro = new DFRobot_BMI160();

    if (_gyro->softReset() != BMI160_OK) {
        LOG_GYRO_ERR("Reset Failed");
        while(1);
    }
    if (_gyro->I2cInit(_addr) != BMI160_OK) {
        LOG_GYRO_ERR("Init Failed");
        while(1);
    }

    LOG_GYRO("BMI160 Ready");

}

void Gyro::loop() {
    int16_t accelGyro[6] = {0};

    if (_gyro->getAccelGyroData(accelGyro) != 0) return ;

    _gx = _alpha * accelGyro[0] + (1 - _alpha) * _gx;
    _gy = _alpha * accelGyro[1] + (1 - _alpha) * _gy;
    _gz = _alpha * accelGyro[2] + (1 - _alpha) * _gz;
    _ax = _alpha * accelGyro[3] + (1 - _alpha) * _ax;
    _ay = _alpha * accelGyro[4] + (1 - _alpha) * _ay;
    _az = _alpha * accelGyro[5] + (1 - _alpha) * _az;

}

float Gyro::getTiltX() const {
    float ax_g = _ax / 16384.0f;
    float az_g = _az / 16384.0f;
    return atan2(ax_g, az_g) * 180.0f / PI;
}

float Gyro::getTiltY() const {
    float ay_g = _ay / 16384.0f;
    float az_g = _az / 16384.0f;
    return atan2(ay_g, az_g) * 180.0f / PI;
}

bool Gyro::isTilted(float threshold) const {
    return abs(getTiltX()) > threshold || abs(getTiltY()) > threshold;
}