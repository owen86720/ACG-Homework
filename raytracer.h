#include "triangle.h"
#include "sphere.h"

#include <vector>

class raytracer
{
public:
    raytracer() {}
    raytracer(std::vector<sphere> S, std::vector<triangle> T, vec3 li) : _spheres(S), _triangles(T), _light(li) {}

    bool isShadow(vec3 hitP)
    {
        ray hitp2light = ray(hitP, _light - hitP);

        for (int s = 0; s < _spheres.size(); ++s)
        {
            if (_spheres.at(s).hit(hitp2light))
                return true;
        }

        for (int s = 0; s < _triangles.size(); ++s)
        {
            if (_triangles.at(s).hit(hitp2light))
                return true;
        }
        return false;
    }

    Pixel raytrace(ray y, int count)
    {
        float ref;
        if (count == 0)
            return Pixel{0, 0, 0};

        Pixel closest;
        auto closestLen = 99999999;
        for (int s = 0; s < _spheres.size(); ++s)
        {
            if (_spheres.at(s).hit(y))
            {
                auto temp = (_light - _spheres.at(s).hitPoint).length();
                if (temp < closestLen)
                {
                    closestLen = temp;
                    closest = _spheres.at(s).phongLighting(_light, y.direction());
                    _hitP = _spheres.at(s).hitPoint;
                    _hitPNor = _spheres.at(s).normal;
                    ref = _spheres.at(s).getReflect();
                }
            }
        }

        for (int s = 0; s < _triangles.size(); ++s)
        {
            if (_triangles.at(s).hit(y))
            {
                auto temp = (_light - _triangles.at(s).hitPoint).length();
                if (temp < closestLen)
                {
                    closestLen = temp;
                    closest = _triangles.at(s).phongLighting(_light, y.direction());
                    _hitP = _triangles.at(s).hitPoint;
                    _hitPNor = _triangles.at(s).normal;
                    ref = _triangles.at(s).getReflect();
                }
            }
        }
        if (closestLen != 99999999)
        {
            //ax+by+cz=d
            auto d = _hitPNor * _hitP;
            auto temp = _hitP + y.direction();
            auto len = (temp * _hitPNor - d) / _hitPNor.length();
            auto b = temp - 2 * len * _hitPNor / _hitPNor.length();
            ray next = ray(_hitP, b - _hitP);
            if (!isShadow(_hitP))
                return closest + raytrace(next, count - 1) * ref;
            else
                return Pixel{0, 0, 0} + raytrace(next, count - 1) * ref;
        }
        else
            return Pixel{0, 0, 0};
    }

    // Pixel Run(int count, ray firstR)
    // {
    //     return raytrace(firstR, count);
    // }

private:
    std::vector<sphere> _spheres;
    std::vector<triangle> _triangles;
    vec3 _light;
    vec3 _hitP;
    vec3 _hitPNor;
};