#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"

class triangle : public hittable
{
public:
    triangle() {}
    triangle(vec3 p1, vec3 p2, vec3 p3) : a(p1), b(p2), c(p3)
    {
        normal = (p2 - p1) ^ (p3 - p1);
        normal[0] = normal[0] * normal[0];
        normal[1] = normal[1] * normal[1];
        normal[2] = normal[2] * normal[2];
        normal = normal.normalize();
    };

    virtual bool hit(const ray r);

public:
    vec3 a;
    vec3 b;
    vec3 c;
    Pixel color;
};

bool triangle::hit(const ray r)
{
    vec3 u = b - a;
    vec3 v = c - a;

    if (r.direction() * normal == 0)
        return false;

    mat3 temp(u, v, -r.direction());
    temp = temp.transpose();
    temp = temp.inverse();
    vec3 temp2 = r.origin() - a;
    auto s1 = temp[0] * temp2;
    auto s2 = temp[1] * temp2;
    auto t = temp[2] * temp2;

    hitPoint = r.at(t);
    if ((s1 < 0) || (s2 < 0) || (s1 > 1) || (s2 > 1) || (t <= 0) || (s1 + s2 > 1))
        return false;

    return true;
}

#endif