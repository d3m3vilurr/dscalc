#include "common.h"
//#define pi() 3.1415926535897931
//#define e() 2.7182818284590451

Num eval1(const char *expr) {
    //return 0; //stub
    Num n;
    n.type = 1;
    n.v.f = 10.101010101;
    return n;
}
Num eval2(const char *expr, Num x) {
    Num n;
    n.type = 1;
    n.v.f = 10.101010101;
    return n;
    //return 0;
}


float add(float a, float b) {
    return a + b;
}

float sub(float a, float b) {
    return a - b;
}

float mul(float a, float b) {
    return a * b;
}

float div(float a, float b) {
    return a / b;
}

float rad2deg(float a) {
    return a * 180. / PI;
}

float deg2rad(float a) {
    return a * PI / 180.;
}
