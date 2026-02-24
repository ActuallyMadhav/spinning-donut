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

void normalize(std::vector<float>& v){
    float magnitude = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);

    if(!magnitude) return;

    v[0] /= magnitude;
    v[1] /= magnitude;
    v[2] /= magnitude;
}

char shader(std::vector<float>& coords, float theta, float phi){
    std::vector<float> surfaceNormal = {
        cos(theta)*cos(phi),
        cos(theta)*sin(phi),
        sin(theta)
    };

    std::vector<float> lightVector = {0, 1, -1};

    // need to normalize vectors so length is <= 1
    normalize(surfaceNormal);
    normalize(lightVector);

    // brightness = dot product of surface normal and light vector
    
    /*
    dot product:
    large positive: bright (facing light)
    near zero: medium (surface sideways)
    negative: dark (facing away)
    */
    
    float brightness = surfaceNormal[0]*lightVector[0] + surfaceNormal[1]*lightVector[1] + surfaceNormal[2]*lightVector[2];

    /*
    need to make brightness return value between 0 and 1
    if brightness < 0 set to 0
    if brightness > 1 set to 1
    */

    if(brightness > 1) brightness = 1;
    if(brightness < 0) brightness = 0;

    std::string gradient = ".,-~:;=!*#$@";
    int n = gradient.size();

    // converting brightness into an index
    int index = brightness*(n-1);
    char pixel = gradient[index];

    return pixel;
}



int main(){

    int rows = 22, cols = 80;
    char init = ' ';
    float R = 2, r = 1;
    float A = 0, B = 0;    

    for(;;){
        std::vector<std::vector<char>> screen(rows, std::vector<char>(cols, init));

        for(float theta = 0; theta < 2*pi; theta+= 0.07){
            for(float phi = 0; phi < 2*pi; phi += 0.02){
                std::vector<float> coords = coords3D(theta, phi, R, r);
                std::vector<float> coords_rotatedX = rotateX(coords, A);
                std::vector<float> coords_rotatedZ = rotateZ(coords_rotatedX, B);
                std::vector<float> coords_projected = projection(coords_rotatedZ);
                std::vector<float> coords_scaled = scaleAndShift(coords_projected);

                int x = (int)coords_scaled[0];
                int y = (int)coords_scaled[1];
                if(x < 0 || x > cols-1 || y < 0 || y > rows-1) continue;

                char pixel = shader(coords_rotatedZ, theta, phi);
                screen[y][x] = pixel;
                //screen[y][x] = '.';
                //std::cout << screen[y][x] << ' ';

            }
        }
        
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                std::cout << screen[i][j];
            }
            std::cout << '\n';
        }
        std::cout << "\x1b[H";
        usleep(50000);
        A += 0.04;
        B += 0.02;
    }
    
    return 0;
}