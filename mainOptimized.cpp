#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unistd.h>

#define pi 3.141592654

// brightness scale (lowest to highest): .,-~:;=!*#$@

std::array<float,3> coords3d(float theta, float phi, float R, float r);

std::array<float,3> rotateX(std::array<float,3>& coords, float A);

std::array<float,3> rotateZ(std::array<float,3>& coords, float B);

std::array<float,2> projection(std::array<float,3>& coords);

std::array<float,2> scaleAndShift(std::array<float,2>& coords);

void normalize(std::array<float,3>& v);

char shader(float theta, float phi);

int main(){
    

    return 0;
}
