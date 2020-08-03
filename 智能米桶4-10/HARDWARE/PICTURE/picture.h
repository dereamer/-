#ifndef __PICTURE_H
#define __PICTURE_H

#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

void show_picture(const u8 *filename,u16 x,u16 y,u16 width,u16 height);
#endif

