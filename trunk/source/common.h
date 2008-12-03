#ifndef __COMMON_H__
#define __COMMON_H__

#include <PA9.h>
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

Num eval1(const char*);
Num eval2(const char*, Num x);

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
