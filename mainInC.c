#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define pi M_PI

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

vec3 rotateX(const vec3 coords, float A){
    vec3 transformed;
    transformed.y = coords.y*cos(A) - coords.z*sin(A);
    transformed.z = coords.y*sin(A) + coords.z*cos(A);
    transformed.x = coords.x;
    return transformed;
}

vec3 rotateZ(const vec3 coords, float B){
    vec3 transformed;
    transformed.x = coords.x*cos(B) - coords.y*sin(B);
    transformed.y = coords.x*sin(B) + coords.y*cos(B);
    transformed.z = coords.z;
    return transformed;
}

vec2 projection(const vec3 coords){
    vec2 projected;
    int k = 5;

    projected.x = coords.x / (coords.z + k);
    projected.y = coords.y / (coords.z + k);

    return projected;
}

vec2 scaleAndShift(const vec2 coords){
    vec2 res;

    res.x = (coords.x * 30) + 40;
    res.y = (coords.y * 15) + 12;

    return res;
}

void normalize(vec3* v){
    float magnitude = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
    if(!magnitude) return;

    v->x /= magnitude;
    v->y /= magnitude;
    v->z /= magnitude;
}

char shader(vec3 coords, float theta, float phi){
    vec3 surfaceNormal = {
        cos(theta)*cos(phi),
        cos(theta)*sin(phi),
        sin(theta)
    };

    vec3 lightVector = {0.0f, 1.0f, -1.0f};

    normalize(&surfaceNormal);   // normalized surface normal
    normalize(&lightVector);     // normalized light vector

    float brightness = surfaceNormal.x*lightVector.x + surfaceNormal.y*lightVector.y + surfaceNormal.z*lightVector.z;

    if(brightness > 1) brightness = 1;
    if(brightness < 0) brightness = 0;

    char gradient[] = ".,-~:;=!*#$@";
    int n = sizeof(gradient) / sizeof(gradient[0]);

    int index = brightness*(n-1);
    char pixel = gradient[index];

    return pixel;
}

int main(){

    int rows = 22, cols = 80;
    char init = ' ';
    float R = 2, r = 1;
    float A = 0, B = 0;
    char screen[22][80];

    for(;;){
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                screen[i][j] = ' ';
            }
        }

        for(float theta = 0; theta < 2*pi; theta += 0.07){
            for(float phi = 0; phi < 2*pi; phi += 0.02){
                vec3 coords = generateCoords3D(theta, phi, R, r);
                vec3 coords_rotatedX = rotateX(coords, A);
                vec3 coords_rotatedZ = rotateZ(coords_rotatedX, B);
                vec2 coords_projected = projection(coords_rotatedZ);
                vec2 coords_scaled = scaleAndShift(coords_projected);


                int x = (int)coords_scaled.x;
                int y = (int)coords_scaled.y;

                if(x < 0 || x > cols-1 || y < 0 || y > rows-1) continue;

                char pixel = shader(coords_rotatedZ, theta, phi);
                screen[y][x] = pixel;
            }
        }

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                printf("%c", screen[i][j]);
            }
            printf("\n");
        }

        printf("\x1b[H");
        usleep(50000);
        A+=0.04;
        B+=0.02;
    }

    return 0;
}