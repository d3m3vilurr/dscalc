#include "common.h"
#define pi() 3.1415926535897931
#define e() 
float calc(const char *expr) {
    return add(10, 20); //stub
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
    return a/b;
}

float rad2deg(float a) {
    return a * 180. / PI;
}

float deg2rad(float a) {
    return a * PI / 180.;
}
