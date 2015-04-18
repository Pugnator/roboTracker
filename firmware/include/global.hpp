#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "io_macro.hpp"

typedef void* HANDLE;

#include "usart.hpp"
#include "log.hpp"
#include "spi.hpp"
#include "gps.hpp"
#include "gsm.hpp"
#include "i2c.hpp"
#include "rtc.hpp"
#include "timers.hpp"
#include "sdcard.hpp"
#include "config.hpp"
#include "text.hpp"

/* FAT32 */
#include "ff.h"
#include "diskio.h"



#define LED C,13,SPEED_50MHz

#define RX1 A,10,SPEED_50MHz
#define TX1 A,9,SPEED_50MHz
#define RX2 A,3,SPEED_50MHz
#define TX2 A,2,SPEED_50MHz

void assert ( int value );

typedef struct RMC
{
	float lat;
	float lon;
	int utc;
} RMC;

extern RMC rmc;
