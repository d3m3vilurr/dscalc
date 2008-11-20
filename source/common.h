#ifndef __COMMON_H__
#define __COMMON_H__

#include <PA9.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UP_LCD 1
#define DOWN_LCD 0

#define BG_0 0
#define BG_1 1
#define BG_2 2
#define BG_3 3

#define COLOR256 1
#define MAXCOL 32
#define MAXROW 24

float calc(const char*);
//void plot(const char*, float, float);

float add(float, float);
float sub(float, float);
float mul(float, float);
float div(float, float);

#define PI 3.1415926535897931
#define E  2.7182818284590451

float rad2deg(float);
float deg2rad(float);

#ifdef __cplusplus
}
#endif
#endif
