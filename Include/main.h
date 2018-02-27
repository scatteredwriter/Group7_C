#ifndef _MAIN
#include "stdio.h"
#include "lcd.h"
#include <pthread.h>
#include <unistd.h>
#include "ui.h"
#include "serial.h"
#define _MAIN

void ButtonListener(struct ts_sample *p);
void *PointerRoutine(void *arg);

#endif