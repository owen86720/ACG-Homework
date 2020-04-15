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

    std::ifstream ifile("hw2_input.txt");
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
            horizontal = (view * 20 ^ U).normalize();
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

    raytracer tr = raytracer(Spheres, Triangles, light);
    for (int j = 0; j < ny; j++)
    {
        for (int i = 0; i < nx; i++)
        {
            scrLoc = scrCenter - i * horizontal - j * vertical;
            //cout << scrLoc << "\n";
            ray ray(eye, scrLoc - eye);
            img.writePixel(i, j, tr.raytrace(ray));
        }
    }
    char filename[] = "output.ppm";
    img.outputPPM(filename);
    return 0;
}