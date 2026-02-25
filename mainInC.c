#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "vec.h"

// brightness scale (lowest to highest): .,-~:;=!*#$@

vec3 coords3D(float theta, float phi, float R, float r){
    vec3 coords;

    coords.x = (R + r*cos(theta)) * cos(phi);
    coords.y = (R + r*cos(theta)) * sin(phi);
    coords.z = r*sin(theta);

    return coords;
}

int main(){

    vec3 coords = coords3D(0.0, 0.0, 2.0, 1.0);

    printf("%f%f%f", coords.x, coords.y, coords.z);

    return 0;
}