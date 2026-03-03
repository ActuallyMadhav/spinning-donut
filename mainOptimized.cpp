#include <iostream>
#include <bits/stdc++.h>
#include <cmath>
#include <unistd.h>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/transform.hpp>

#define pi M_PI

// brightness scale (lowest to highest): .,-~:;=!*#$@

glm::vec3 coords3d(float theta, float phi, float R, float r){
    float ct = cos(theta);
    float st = sin(theta);
    float cp = cos(phi);
    float sp = sin(phi);

    float x = (R + r*ct) * cp;
    float y = (R + r*ct) * sp;
    float z = r*st;

    return {x, y, z};
}

// GLM provides transformation and rotation functions - glm::rotate(angle, vec)
glm::vec3 my_rotateX(const glm::vec3& coords, float A){
    float ca = cos(A);
    float sa = sin(A);

    float y_transformed = coords[1]*ca - coords[2]*sa;
    float z_transformed = coords[1]*sa + coords[2]*ca;
    float x_transformed = coords[0]; // x remains unchanged

    return {x_transformed, y_transformed, z_transformed};
}

glm::vec3 my_rotateZ(const glm::vec3& coords, float B){
    float cb = cos(B);
    float sb = sin(B);

    float x_transformed = coords[0]*cb - coords[1]*sb;
    float y_transformed = coords[0]*sb + coords[1]*cb;
    float z_transformed = coords[2]; // z remains unchanged

    return {x_transformed, y_transformed, z_transformed};
}

// GLM based rotation functions:
glm::vec3 rotateX(const glm::vec3& coords, float A){
    glm::mat4 rotationMatrix = glm::rotate(A, glm::vec3(1.0f, 0.0f, 0.0f)); // (1,0,0) -> rotate on x
    glm::vec4 transformed = rotationMatrix * glm::vec4(coords, 1.0f);   // apply rot matrix to vector. matrix is 4x4 so need vec4
    return glm::vec3(transformed);  // return vec3
}

glm::vec3 rotateZ(const glm::vec3& coords, float B){
    glm::mat4 rotationMatrix = glm::rotate(B, glm::vec3(0.0f, 0.0f, 1.0f)); // (0,0,1) -> rotate on z
    glm::vec4 transformed = rotationMatrix * glm::vec4(coords, 1.0f);   // apply rot matrix to vector. matrix is 4x4 so need vec4
    return glm::vec3(transformed);  // return vec3
}

glm::vec2 projection(const glm::vec3& coords){
    int K = 5;

    float X = coords[0] / (coords[2] + K);
    float Y = coords[1] / (coords[2] + K);

    return {X, Y};
}

glm::vec2 scaleAndShift(const glm::vec2& coords){
    float X_scaled = coords[0] * 30;
    float Y_scaled = coords[1] * 15;

    float X_final = X_scaled + 40;
    float Y_final = Y_scaled + 12;

    return {X_final, Y_final};
}

// not required, GLM provides glm::normalize(v)
void normalize(glm::vec3& v){
    float magnitude = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);

    if(!magnitude) return;

    v[0] /= magnitude;
    v[1] /= magnitude;
    v[2] /= magnitude;
}

char shader(float theta, float phi){
    float ct = cos(theta), cp = cos(phi);
    float st = sin(theta), sp = sin(phi);
    
    glm::vec3 surfaceNormal = {
        ct * cp,
        ct * sp,
        st
    };

    glm::vec3 lightVector = {0.0f, 1.0f, -1.0f};

    surfaceNormal = glm::normalize(surfaceNormal);
    lightVector = glm::normalize(lightVector);

    float brightness = glm::dot(surfaceNormal, lightVector);

    // if(brightness > 1) brightness = 1;
    // if(brightness < 0) brightness = 0;
    brightness = glm::clamp(brightness, 0.0f, 1.0f);

    std::string gradient = ".,-~:;=!*#$@";
    int n = gradient.size();

    int index = brightness*(n-1);
    char pixel = gradient[index];

    return pixel;
}

int main(){
    
    int rows = 22, cols = 80;
    char init = ' ';
    float R = 2, r = 1;
    float A = 0, B = 0;

    std::vector<std::vector<char>> screen(rows, std::vector<char>(cols, init));

    for(;;){
        // reset buffer
        for(int i = 0; i < rows; i++){
            std::fill(screen[i].begin(), screen[i].end(), init);
        }

        // donut coords
        for(float theta = 0; theta < 2*pi; theta += 0.09){  // had to use slightly larger step size to account for extra calcs per frame
            for(float phi = 0; phi < 2*pi; phi += 0.04){
                glm::vec3 coords = coords3d(theta, phi, R, r);
                glm::vec3 coords_rotatedX = rotateX(coords, A);
                glm::vec3 coords_rotatedZ = rotateZ(coords_rotatedX, B);
                glm::vec2 coords_projected = projection(coords_rotatedZ);
                glm::vec2 coords_scaled = scaleAndShift(coords_projected);

                int x = (int)coords_scaled[0];
                int y = (int)coords_scaled[1];
                if(x < 0 || x > cols-1 || y < 0 || y > rows-1) continue;

                char pixel = shader(theta, phi);
                screen[y][x] = pixel;
            }
        }

        std::cout << "\x1b[H";  // clear screen
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                std::cout << screen[i][j];
            }
            std::cout << '\n';
        }

        usleep(50000);
        A += 0.04;
        B += 0.02;
    }

    return 0;
}
