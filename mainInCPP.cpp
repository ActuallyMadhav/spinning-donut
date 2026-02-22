#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>

#define pi 3.141592654

// brightness scale (lowest to highest): .,-~:;=!*#$@

std::vector<float> coords3D(float theta, float phi, float R, float r){
    //theta - angle that goes around tube cross section
    // phi - angle that goes around the donut hole
    // R - distance from centre of donut hole to centre of ring
    // r - radius of the tube cross section

    float x = (R + r*cos(theta)) * cos(phi);
    float y = (R + r*cos(theta)) * sin(phi);
    float z = r*sin(theta);

    return {x, y, z};
}

/*
    in every use of coords[i], i = 0,1,2
    coords[0] = x coord
    coords[1] = y coord
    coords[2] = z coord
*/ 

std::vector<float> rotateX(std::vector<float>& coords, float A){
    // MUST USE THE RESULTS FROM coords3d() FUNCTION !!!!!

    // coords is the raw 3d coords

    float y_transformed = coords[1]*cos(A) - coords[2]*sin(A);
    float z_transformed = coords[1]*sin(A) + coords[2]*cos(A);
    float x_transformed = coords[0]; // x remains unchanged

    return {x_transformed, y_transformed, z_transformed};
}

std::vector<float> rotateZ(std::vector<float>& coords, float B){
    // MUST USE THE RESULTS FROM rotateX() FUNCTION !!!!!

    // coords is the raw 3d coords but rotated by angle A on the X-axis

    float x_transformed = coords[0]*cos(B) - coords[1]*sin(B);
    float y_transformed = coords[0]*sin(B) + coords[1]*cos(B);
    float z_transformed = coords[2]; // z remains unchanged

    return {x_transformed, y_transformed, z_transformed};
}

std::vector<float> projection(std::vector<float>& coords){
    // MUST USE THE RESULTS FROM rotateZ() FUNCTION !!!!!

    // coords is the raw 3d coords but rotated by angle A on X-axis and angle B on Z-axis

    int K = 5;  // constant that shifts everything forward to prevent division by zero
    // K can take any value, large K -> smaller and flatter, small k-> bigger and distorted

    // since we are projecting 3d onto 2d, only X and Y coords are required
    float X = coords[0] / (coords[2] + K);
    float Y = coords[1] / (coords[2] + K);

    return {X, Y};
}

std::vector<float> scaleAndShift(std::vector<float>& coords){
    // uses projected coords

    // scaling to spread out the coordinates
    float  X_scaled = coords[0]*30;
    float Y_scaled = coords[1]*15;

    // shifting by adding an offset to make them centered
    float X_final = X_scaled + 40;
    float Y_final = Y_scaled + 12;

    return {X_final, Y_final};
}

char shader(std::vector<float>& coords){
    
}

int main(){

    float R = 2, r = 1;
    float A = 0, B = 0;

    for(float i = 0; i < 2*pi; i += 0.07){
        for(float j = 0; j < 2*pi; j += 0.02){
            std::vector<float> coords = coords3D(i,j,R,r);
            std::vector<float> coords_rotatedX = rotateX(coords, A);
            std::vector<float> coords_rotatedZ = rotateZ(coords_rotatedX, B);   // final 3D coords

            std::vector<float> coords_projected = projection(coords_rotatedZ);  // projecting 3d coordinates onto 2d screen

            std::vector<float> coordsScaled = scaleAndShift(coords_projected);  // scaling and shifting coords to align with screen
        
            // printing coords just for sanity
            // std::cout << "(" << coords[0] << ", " << coords[1] << ", " << coords[2] << ")" << '\n';
            //std::cout << "[" << coords_rotatedZ[0] << ", " << coords_rotatedZ[1] << ", " << coords_rotatedZ[2] << "]" << '\n';
            //std::cout << "[" << coords_projected[0] << ", " << coords_projected[1] << "]" << '\n';
            std::cout << "[" << coordsScaled[0] << ", " << coordsScaled[1] << "]" << '\n';
        }
        A += 0.04;
        B += 0.02;
    }

    return 0;
}