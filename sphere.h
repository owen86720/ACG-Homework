#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable
{
public:
    sphere() {}
    sphere(vec3 cen, double r) : center(cen), radius(r){};

    virtual bool hit(const ray r);

public:
    vec3 center;
    double radius;
};

bool sphere::hit(const ray r)
{
    vec3 oc = r.origin() - center;
    auto a = r.direction() * r.direction();
    auto b = 2.0 * oc * r.direction();
    auto c = oc * oc - radius * radius;
    auto discriminant = b * b - 4 * a * c;

    if (discriminant > 0)
    {
        auto t = (-b - sqrt(discriminant)) / 2 / a;
        if (t >= 0)
        {
            hitPoint = r.at(t);
            normal = (hitPoint - center).normalize();
            return true;
        }
    }
    return false;
}

#endif