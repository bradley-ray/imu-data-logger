#!/usr/bin/env python3

import sys
import serial
import csv


ACCEL_SENS = 8192.0
GYRO_SENS = 65.5


def to_int(sens, num):
    d = int.from_bytes(num, 'big')
    if d & (1 << 15):
        d -= (1 << 16)
    return d / sens


if __name__ == '__main__':
    filename = 'log.csv'
    baud = 115200
    port = ''

    if len(sys.argv) > 2:
        port = sys.argv[1]
    else:
        print("Please provide a serial port")
        exit(1)

    if len(sys.argv) > 3:
        baud = int(sys.argv[2])
    else:
        print("using default baud: `115200`")

    if len(sys.argv) > 3:
        filename = sys.argv[3]
    else:
        print("using default filename: `log.csv`")

    com = serial.Serial(port, baudrate=baud)
    with open(filename, 'w') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['accel_x', 'accel_y', 'accel_z',
                         'gyro_x', 'gyro_y', 'gyro_z'])

        data = [0 for i in range(6)]
        while True:
            if com.read(1) == '$':
                raw = com.read(12)
                for i in range(0, 12, 2):
                    sens = ACCEL_SENS if i < 6 else GYRO_SENS
                    data[i] = to_int(sens, raw[i] + raw[i+1])
                writer.writerow(data)
