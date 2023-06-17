#include "mpu6050.h"

uint8_t mpu_reset_enable(uint8_t* tx_buff) {
    tx_buff[0] = 0;
    return mpu_write_reg(MPU_PWR_MGMT_REG, tx_buff, 1);
}

uint8_t mpu_reset_disable(uint8_t* tx_buff) {
    tx_buff[0] = 0;
    return mpu_write_reg(MPU_PWR_MGMT_REG, tx_buff, 1);
}

uint8_t mpu_int_cfg(uint8_t* tx_buff) {
    tx_buff[0] = 0x20;
    return mpu_write_reg(MPU_INT_CFG_REG, tx_buff, 1);
}

uint8_t mpu_int_on(uint8_t* tx_buff) {
    tx_buff[0] = 0x01;
    return mpu_write_reg(MPU_INT_EN_REG, tx_buff, 1);
}

uint8_t mpu_int_off(uint8_t* tx_buff) {
    tx_buff[0] = 0x00;
    return mpu_write_reg(MPU_INT_EN_REG, tx_buff, 1);
}

uint8_t mpu_accel_cfg(uint8_t sens, uint8_t* tx_buff) {
    tx_buff[0] = sens << 3;
    return mpu_write_reg(MPU_ACCEL_CFG_REG, tx_buff, 1);
}

uint8_t mpu_gyro_cfg(uint8_t sens, uint8_t* tx_buff) {
    tx_buff[0] = sens << 3;
    return mpu_write_reg(MPU_ACCEL_CFG_REG, tx_buff, 1);
}

uint8_t mpu_init(uint8_t a_sens, uint8_t g_sens, uint8_t* tx_buff) {
    uint8_t err;

    err = mpu_reset_enable(tx_buff);
    if (err != 0)
        return err;
    err = mpu_reset_disable(tx_buff);
    if (err != 0)
        return err;
    err = mpu_accel_cfg(a_sens, tx_buff);
    if (err != 0)
        return err;
    err = mpu_accel_cfg(g_sens, tx_buff);
    if (err != 0)
        return err;
    err = mpu_int_cfg(tx_buff);
    if (err != 0)
        return err;
    err = mpu_int_on(tx_buff);
    if (err != 0)
        return err;

    return 0;
}

uint8_t mpu_int_status(uint8_t* rx_buff) {
    return mpu_read_reg(MPU_INT_STATUS_REG, rx_buff, 1);
}

uint8_t mpu_read_accel(uint8_t* rx_buff) {
    return mpu_read_reg(MPU_ACCEL_REG, rx_buff, 6);
}

uint8_t mpu_read_gyro(uint8_t* rx_buff) {
    return mpu_read_reg(MPU_GYRO_REG, rx_buff, 6);
}