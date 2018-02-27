#ifndef _SE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

// unsigned char cur_value[8];
long cur_value;
void ReadValue();
void GetValue();

#define _SE
#endif