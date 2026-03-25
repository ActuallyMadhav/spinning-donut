import math
import time
PI = math.pi

def generateCoords3d(theta, phi, R, r):
    ct = math.cos(theta)
    cp = math.cos(phi)
    st = math.sin(theta)
    sp = math.sin(phi)

    x = (R + r*ct) * cp
    y = (R + r*ct) * sp
    z = r*st

    return [x,y,z]

def rotateX(v, A):
    cA = math.cos(A)
    sA = math.sin(A)

    y = v[1]*cA - v[2]*sA
    z = v[1]*sA + v[2]*cA
    x = v[0]

    return [x,y,z]

def rotateZ(v, B):
    cB = math.cos(B)
    sB = math.sin(B)
    x = v[0]*cB - v[1]*sB
    y = v[0]*sB + v[1]*cB
    z = v[2]

    return [x,y,z]

def projection(v):
    k = 5
    x = v[0] / (v[2] + k)
    y = v[1] / (v[2] + k)

    return [x,y]

def scaleAndShift(v):
    x = v[0]*30 + 40
    y = v[1]*15 + 12
    return [x,y]

def normalize(v):
    magnitude = math.sqrt(v[0]**2 + v[1]**2 + v[2]**2)
    if not magnitude:
        return [0,0,0]
    
    v[0] /= magnitude
    v[1] /= magnitude
    v[2] /= magnitude

    return v

def dotProduct(v1, v2):
    res = 0
    for i in range(len(v1)):
        res += v1[i]*v2[i]
    return res


def shader(theta, phi):
    ct = math.cos(theta)
    st = math.sin(theta)
    cp = math.cos(phi)
    sp = math.sin(phi)
    surfaceNormal = [ct*cp, ct*sp, st]
    lightVector = [0.0, 1.0, -1.0]

    surfaceNormal = normalize(surfaceNormal)
    lightVector = normalize(lightVector)

    brightness = dotProduct(surfaceNormal, lightVector)
    
    if brightness > 1:
        brightness = 1
    if brightness < 0:
        brightness = 0
    
    gradient = ".,-~:;=!*#$@"
    n = len(gradient)

    index = int(brightness * (n-1))
    pixel = gradient[index]

    return pixel

def main():
    rows = 22
    cols = 80
    R = 2.0
    r = 1.0
    A = 0.0
    B = 0.0
    
    while True:
        screen = [[' ' for _ in range(cols)] for _ in range(rows)]

        theta = 0
        while theta < 2*PI:
            phi = 0
            while phi < 2*PI:
                coords = generateCoords3d(theta, phi, R, r)
                coords_rotatedX = rotateX(coords, A)
                coords_rotatedZ = rotateZ(coords_rotatedX, B)
                coords_projected = projection(coords_rotatedZ)
                coords_scaled = scaleAndShift(coords_projected)

                x = int(coords_scaled[0])
                y = int(coords_scaled[1])

                if 0 <= x < cols and 0 <= y < rows:
                    pixel = shader(theta, phi)
                    screen[y][x] = pixel
                phi += 0.02
            theta += 0.07

        print("\x1b[H\x1b[2J")   # clear screen first
        for i in range(rows):
            print(''.join(screen[i]))

        time.sleep(0.05)
        A += 0.04
        B += 0.02


if __name__ == '__main__':
    main()