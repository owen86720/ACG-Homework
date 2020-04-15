#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "imageIO.h"

class hittable
{
public:
    vec3 hitPoint;
    vec3 normal;
    void setMaterial(float r, float g, float b, float a, float d, float s, float ex, float ref)
    {
        _Ka = a;
        _Kd = d;
        _Ks = s;
        _ex = ex;
        _Reflect = ref;
        _color = vec3(r * 255, g * 255, b * 255);
    }
    virtual bool hit(const ray r) = 0;
    Pixel phongLighting(vec3 light, vec3 eye2src)
    {
        vec3 L = (light - hitPoint).normalize();
        vec3 V = eye2src.normalize();
        vec3 eyeNor = light + V - L;
        vec3 H = (eyeNor + (light - eyeNor) / 2 - hitPoint).normalize();
        auto Ia = _color;
        auto Id = MAX(L * normal, 0) * _color;
        auto Is = pow(H * normal, _ex) * _color;
        auto I = Ia * _Ka + Id * _Kd + Is * _Ks;

        I[0] = MIN(I[0], 255);
        I[1] = MIN(I[1], 255);
        I[2] = MIN(I[2], 255);

        Pixel colorOut = {(unsigned char)(I[0]), (unsigned char)(I[1]), (unsigned char)(I[2])};

        return colorOut;
    }
    float getReflect()
    {
        return _Reflect;
    }

private:
    vec3 _color;
    float _Ka, _Kd, _Ks;
    float _ex;
    float _Reflect;
};

#endif