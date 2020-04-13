#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "algebra3.h"

#endif

class ray
{
public:
    ray() {}
    ray(const vec3 &origin, const vec3 &direction)
        : orig(origin), dir(direction)
    {
    }

    vec3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    vec3 at(double t) const
    {
        return orig + t * dir;
    }

    vec3 vect()
    {
        return dir - orig;
    }

public:
    vec3 orig;
    vec3 dir;
};