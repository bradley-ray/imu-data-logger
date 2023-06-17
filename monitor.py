#!/usr/bin/env python3
import serial

ACCEL_SENS = 4096.0
GYRO_SENS  = 32.8

def to_int(sens, num):
    #d = high*256 + low
    d = int.from_bytes(num, 'big')
    if d & (1 << 15):
        d -= (1 << 16)
    return d / sens

if __name__ == '__main__':
    com = serial.Serial('/dev/ttyACM0', baudrate=115200)

    data = [0, 0, 0]
    while(True):
        if ord(com.read(1)) == ord('$'):
            if ord(com.read(1)) == 0:
                data[0] = to_int(ACCEL_SENS, com.read(1) + com.read(1))
                data[1] = to_int(ACCEL_SENS, com.read(1) + com.read(1))
                data[2] = to_int(ACCEL_SENS, com.read(1) + com.read(1))
                print('Accel:')
                print('---------')
                print('\tx:', data[0], 'g')
                print('\ty:', data[1], 'g')
                print('\tz:', data[2], 'g')
            elif ord(com.read(1)) == 1:
                data[0] = to_int(GYRO_SENS, com.read(1) + com.read(1))
                data[1] = to_int(GYRO_SENS, com.read(1) + com.read(1))
                data[2] = to_int(GYRO_SENS, com.read(1) + com.read(1))
                print('Gyro:')
                print('---------')
                print('\tx:', data[0], 'deg/s')
                print('\ty:', data[1], 'deg/s')
                print('\tz:', data[2], 'deg/s')

