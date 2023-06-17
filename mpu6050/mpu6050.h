#ifndef __MPU6050_H__
#define __MPU6050_H__

#include <stdint.h>

#define MPU_ACCEL_2g 0
#define MPU_ACCEL_4g 1
#define MPU_ACCEL_8g 2
#define MPU_ACCEL_16g 3

#define MPU_GYRO_250deg 0
#define MPU_GYRO_500deg 1
#define MPU_GYRO_1000deg 2
#define MPU_GYRO_2000deg 3

typedef enum {
    MPU_GYRO_CFG_REG = 0x1b,
    MPU_ACCEL_CFG_REG = 0x1c,
    MPU_INT_CFG_REG = 0x37,
    MPU_INT_EN_REG = 0x38,
    MPU_INT_STATUS_REG = 0x3a,
    MPU_ACCEL_REG = 0x3b,
    MPU_GYRO_REG = 0x43,
    MPU_PWR_MGMT_REG = 0x6b,
} mpu_reg_t;

uint8_t mpu_write_reg(mpu_reg_t reg, uint8_t* tx_buff, uint8_t size);
uint8_t mpu_read_reg(mpu_reg_t reg, uint8_t* rx_buff, uint8_t size);


// mpu6050.c
uint8_t mpu_reset_enable(uint8_t* tx_buff);
uint8_t mpu_reset_disable(uint8_t* tx_buff);
uint8_t mpu_int_on(uint8_t* tx_buff);
uint8_t mpu_int_off(uint8_t* tx_buff);
uint8_t mpu_int_status(uint8_t* rx_buff);
uint8_t mpu_accel_cfg(uint8_t sens, uint8_t* tx_buff);
uint8_t mpu_gyro_cfg(uint8_t sens, uint8_t* tx_buff);
uint8_t mpu_init(uint8_t a_sens, uint8_t g_sens, uint8_t* tx_buff);
uint8_t mpu_read_accel(uint8_t* rx_buff);
uint8_t mpu_read_gyro(uint8_t* rx_buff);

#endif