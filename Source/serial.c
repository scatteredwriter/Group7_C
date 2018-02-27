#include "serial.h"

void ReadValue(unsigned char *value)
{

    //1. 打开串口设备
    int serial_fd = open("/dev/s3c2410_serial3", O_RDWR);
    if (serial_fd == -1)
    {
        printf("main:open serial error!\n");
    }

    //2.设置串口

    //获取串口属性
    struct termios termios_old;     //为了在后面进行关闭串口的时候，恢复原来的设置
    struct termios termios_current; //为了在原来的基础上改变串口的设置（属性）

    tcgetattr(serial_fd, &termios_old);
    tcgetattr(serial_fd, &termios_current);

    //设置位raw模式
    cfmakeraw(&termios_current);
    //设置波特率
    cfsetspeed(&termios_current, B115200);

    //设置bit位 8位
    termios_current.c_cflag &= ~CSIZE; //11 0000
    termios_current.c_cflag |= CS8;    //11 0000

    //设置停止位
    termios_current.c_cflag &= ~CSTOPB; //

    //关闭奇偶校验
    termios_current.c_cflag &= ~PARENB;

    termios_current.c_cflag &= ~CRTSCTS;

    tcflush(serial_fd, TCIOFLUSH);

    termios_current.c_cc[VTIME] = 0;
    termios_current.c_cc[VMIN] = 1;

    tcsetattr(serial_fd, TCSANOW, &termios_current);

    read(serial_fd, value, 8);

    tcsetattr(serial_fd, TCSANOW, &termios_old);
    close(serial_fd);
}

void GetValue()
{
    unsigned char value[8];
    ReadValue(value);
    cur_value = (value[0] - 48) * 10000000 + (value[1] - 48) * 1000000 + (value[2] - 48) * 100000 + (value[3] - 48) * 10000 + (value[4] - 48) * 1000 + (value[5] - 48) * 100 + (value[6] - 48) * 10 + (value[7] - 48);
}
