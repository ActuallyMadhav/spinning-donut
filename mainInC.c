#include <stdio.h>
#include <math.h>
#include <unistd.h>

typedef struct{float x,y;} vec2;
typedef struct{float x,y,z;} vec3;

// brightness scale (lowest to highest): .,-~:;=!*#$@

vec3 generateCoords3D(float theta, float phi, float R, float r){
    vec3 coords;

    coords.x = (R + r*cos(theta)) * cos(phi);
    coords.y = (R + r*cos(theta)) * sin(phi);
    coords.z = r*sin(theta);

    return coords;
}

vec3 rotateX(vec3 coords, float A){
    vec3 transformed;
    transformed.y = coords.y*cos(A) - coords.z*sin(A);
    transformed.z = coords.y*sin(A) + coords.z*cos(A);
    transformed.x = coords.x;
    return transformed;
}

int main(){

    vec3 coords = generateCoords3D(0.0, 0.0, 2.0, 1.0);

    printf("%.2f %.2f %.2f", coords.x, coords.y, coords.z);

    return 0;
}