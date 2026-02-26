#include <iostream>
#include <cmath>
#include <unistd.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#define pi M_PI

// brightness scale (lowest to highest): .,-~:;=!*#$@

glm::vec3 coords3d(float theta, float phi, float R, float r){
    float x = (R + r*cos(theta)) * cos(phi);
    float y = (R + r*cos(theta)) * sin(phi);
    float z = r*sin(theta);

    return {x, y, z};
}

// GLM provides transformation and rotation functions - glm::rotate(angle, vec)
glm::vec3 my_rotateX(const glm::vec3& coords, float A){
    float y_transformed = coords[1]*cos(A) - coords[2]*sin(A);
    float z_transformed = coords[1]*sin(A) + coords[2]*cos(A);
    float x_transformed = coords[0]; // x remains unchanged

    return {x_transformed, y_transformed, z_transformed};
}

glm::vec3 my_rotateZ(const glm::vec3& coords, float B){
    float x_transformed = coords[0]*cos(B) - coords[1]*sin(B);
    float y_transformed = coords[0]*sin(B) + coords[1]*cos(B);
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
    glm::vec3 surfaceNormal = {
        cos(theta) * cos(phi),
        cos(theta) * sin(phi),
        sin(theta)
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
    


    return 0;
}
