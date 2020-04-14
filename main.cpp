
#include "algebra3.h"
#include "imageIO.h"
#include "ray.h"

#include <vector>
#include <fstream>

#define PI 3.1415926535898
#define Tan(th) tan(PI / 180 * (th))
//using namespace std;

int nx, ny;
float angle;

vec3 eye, scrCenter, horizontal, vertical, view;
std::vector<vec4> Sphere;
std::vector<vec3> Triangle; //,Screen

void readFile()
{
    float angle, SOx, SOy, SOz, Sr;
    float Ax, Ay, Az, Bx, By, Bz, Cx, Cy, Cz;
    vec3 U;

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
            horizontal = (view * 8 ^ U).normalize();
            vertical = (horizontal ^ view).normalize();
            std::cout << "h:" << horizontal << "\nv:" << vertical << "\n";
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
        case 'S':
            ifile >> SOx >> SOy >> SOz >> Sr;
            Sphere.push_back(vec4(SOx, SOy, SOz, Sr));
            break;
        case 'T':
            ifile >> Ax >> Ay >> Az >> Bx >> By >> Bz >> Cx >> Cy >> Cz;
            Triangle.push_back(vec3(Ax, Ay, Az));
            Triangle.push_back(vec3(Bx, By, Bz));
            Triangle.push_back(vec3(Cx, Cy, Cz));
            break;
        }
    }
}

bool hitSphere(int index, ray r)
{
    vec4 sphere = Sphere.at(index);
    vec3 center(sphere[0], sphere[1], sphere[2]);
    float radius = sphere[3];
    //vec3 fuck = r.origin();
    //cout << fuck << " " << eye << "\n";
    vec3 oc = r.origin() - center;
    auto a = r.direction() * r.direction();
    auto b = 2.0 * oc * r.direction();
    auto c = oc * oc - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

bool hitTriangle(int index, ray r)
{
    vec3 a = Triangle.at(index * 3);
    vec3 b = Triangle.at(index * 3 + 1);
    vec3 c = Triangle.at(index * 3 + 2);
    //cout << "a:" << a << "\nb:" << b << "\nc:" << c << "\n";

    vec3 u = b - a;
    vec3 v = c - a;
    vec3 n = u ^ v;
    //cout << "u:" << u << "\nv:" << v << "\nn:" << n << "\n";
    if (r.direction() * n == 0)
        return false;

    mat3 temp(u, v, -r.direction());
    temp = temp.transpose();
    //cout << temp << "\n";
    temp = temp.inverse();
    vec3 temp2 = eye - a;
    auto s1 = temp[0] * temp2;
    auto s2 = temp[1] * temp2;
    auto t = temp[2] * temp2;
    //vec3 fucj = temp * temp2;
    //cout << s1 << " " << s2 << " " << t << "\n";
    if ((s1 < 0) || (s2 < 0) || (s1 > 1) || (s2 > 1) || (t <= 0))
        return false;

    return true;
}
int main()
{
    readFile();
    ColorImage img;
    img.init(nx, ny);
    Pixel white = {255, 255, 255};
    vec3 scrLoc;
    for (int j = 0; j < ny; j++)
    {
        for (int i = 0; i < nx; i++)
        {
            scrLoc = scrCenter - i * horizontal - j * vertical;
            //cout << scrLoc << "\n";
            ray ray(eye, scrLoc - eye);
            for (int s = 0; s < Sphere.size(); ++s)
            {
                //printf("123");
                if (hitSphere(s, ray))

                    img.writePixel(i, j, white);
            }

            for (int s = 0; s < Triangle.size() / 3; ++s)
            {
                if (hitTriangle(s, ray))
                    img.writePixel(i, j, white);
            }
        }
    }
    char filename[] = "output.ppm";
    img.outputPPM(filename);
    return 0;
}