#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define pi 3.141582654

typedef struct{
    float x, y
} Vec2;

typedef struct{
    float x,y,z
} Vec3;

// brightness scale (lowest to highest): .,-~:;=!*#$@

Vec3 coords3D(float theta, float phi, float R, float r){
    Vec3 coords;

    coords.x = (R + r*cos(theta)) * cos(phi);
    coords.y = (R + r*cos(theta)) * sin(phi);
    coords.z = r*sin(theta);

    return coords;
}