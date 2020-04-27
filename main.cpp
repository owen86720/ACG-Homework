#include "raytracer.h"
#include "imageIO.h"

#include <vector>
#include <fstream>

#define PI 3.1415926535898
#define Tan(th) tan(PI / 180 * (th))

int nx, ny;

vec3 eye, scrCenter, horizontal, vertical, light;
std::vector<sphere> Spheres;
std::vector<triangle> Triangles;

void readFile()
{
    float angle, SOx, SOy, SOz, Sr;
    float Ax, Ay, Az, Bx, By, Bz, Cx, Cy, Cz;
    float r, g, b, Ka, Kd, Ks, ex, ref;
    vec3 U, view;

    std::ifstream ifile("hw3_input.txt");
    std::string test;
    while (ifile >> test)
    {
        switch (test[0])
        {
        case 'E':
            ifile >> eye[0] >> eye[1] >> eye[2];
            break;
        case 'V':
            ifile >> view[0] >> view[1] >> view[2] >> U[0] >> U[1] >> U[2];
            view = view.normalize() * 60;
            horizontal = (view ^ U).normalize();
            vertical = (horizontal ^ view).normalize();
            //std::cout << "h:" << horizontal << "\nv:" << vertical << "\n";
            break;
        case 'F':
            ifile >> angle;
            horizontal = horizontal * view.length() * Tan(angle / 2);
            vertical = vertical * view.length() * Tan(angle / 2);
            scrCenter = eye + view + horizontal + vertical;
            break;
        case 'R':
            ifile >> nx >> ny;
            horizontal = horizontal / nx * 2;
            vertical = vertical / ny * 2;
            //std::cout << "h:" << horizontal << "\nv:" << vertical << "\n";
            break;
        case 'M':
            ifile >> r >> g >> b >> Ka >> Kd >> Ks >> ex >> ref;
            break;
        case 'S':
            ifile >> SOx >> SOy >> SOz >> Sr;
            Spheres.push_back(sphere(vec3(SOx, SOy, SOz), Sr));
            Spheres.back().setMaterial(r, g, b, Ka, Kd, Ks, ex, ref);
            break;
        case 'T':
            ifile >> Ax >> Ay >> Az >> Bx >> By >> Bz >> Cx >> Cy >> Cz;
            Triangles.push_back(triangle(vec3(Ax, Ay, Az), vec3(Bx, By, Bz), vec3(Cx, Cy, Cz)));
            Triangles.back().setMaterial(r, g, b, Ka, Kd, Ks, ex, ref);
            break;
        case 'L':
            ifile >> Ax >> Ay >> Az;
            light = vec3(Ax, Ay, Az);
            break;
        }
    }
}
int main()
{
    readFile();
    ColorImage img;
    img.init(nx, ny);
    vec3 scrLoc;
    vec3 hitpoint;
    vec3 eyeA = eye + vec3(0.7, 0, 0);
    vec3 eyeB = eye + vec3(0, 0.7, 0);
    raytracer tr = raytracer(Spheres, Triangles, light);
    for (int j = 0; j < ny; j++)
    {
        for (int i = 0; i < nx; i++)
        {
            scrLoc = scrCenter - i * horizontal - j * vertical;
            //cout << scrLoc << "\n";
            ray rayO(eye, scrLoc - eye);
            ray rayA(eyeA, scrLoc - eyeA);
            ray rayB(eyeB, scrLoc - eyeB);
            auto col = tr.raytrace(rayO, 1).mean(tr.raytrace(rayA, 1), tr.raytrace(rayB, 1));
            img.writePixel(i, j, col);
        }
    }
    char filename[] = "output.ppm";
    img.outputPPM(filename);
    return 0;
}