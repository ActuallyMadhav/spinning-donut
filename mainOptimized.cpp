#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unistd.h>
#include <glm/glm.hpp>

#define pi M_PI

// brightness scale (lowest to highest): .,-~:;=!*#$@

std::array<float,3> coords3d(float theta, float phi, float R, float r){
    float x = (R + r*cos(theta)) * cos(phi);
    float y = (R + r*cos(theta)) * sin(phi);
    float z = r*sin(theta);

    return {x, y, z};
}

std::array<float,3> rotateX(const std::array<float,3>& coords, float A){
    float y_transformed = coords[1]*cos(A) - coords[2]*sin(A);
    float z_transformed = coords[1]*sin(A) + coords[2]*cos(A);
    float x_transformed = coords[0]; // x remains unchanged

    return {x_transformed, y_transformed, z_transformed};
}

std::array<float,3> rotateZ(const std::array<float,3>& coords, float B){
    float x_transformed = coords[0]*cos(B) - coords[1]*sin(B);
    float y_transformed = coords[0]*sin(B) + coords[1]*cos(B);
    float z_transformed = coords[2]; // z remains unchanged

    return {x_transformed, y_transformed, z_transformed};
}

std::array<float,2> projection(const std::array<float,3>& coords){
    int K = 5;

    float X = coords[0] / (coords[2] + K);
    float Y = coords[1] / (coords[2] + K);

    return {X, Y};
}

std::array<float,2> scaleAndShift(const std::array<float,2>& coords){
    float X_scaled = coords[0] * 30;
    float Y_scaled = coords[1] * 15;

    float X_final = X_scaled + 40;
    float Y_final = Y_scaled + 12;

    return {X_final, Y_final};
}

void normalize(std::array<float,3>& v){
    float magnitude = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);

    if(!magnitude) return;

    v[0] /= magnitude;
    v[1] /= magnitude;
    v[2] /= magnitude;
}

char shader(float theta, float phi);

int main(){
    
    glm::vec2 a(1.0f, 2.0f);

    std::cout << a.x << a.y;

    return 0;
}
