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

    Pixel raytrace(ray y)
    {
        Pixel closest;
        auto closestLen = 99999999;
        vec3 closestP;
        for (int s = 0; s < _spheres.size(); ++s)
        {
            if (_spheres.at(s).hit(y))
            {
                auto temp = (_light - _spheres.at(s).hitPoint).length();
                if (temp < closestLen)
                {
                    closestLen = temp;
                    closest = _spheres.at(s).phongLighting(_light, y.direction());
                    closestP = _spheres.at(s).hitPoint;
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
                    closestP = _triangles.at(s).hitPoint;
                }
            }
        }
        if (closestLen != 99999999)
        {
            if (!isShadow(closestP))
                return closest;
            else
                return Pixel{0, 0, 0};
        }
        else
            return Pixel{0, 0, 0};
    }

private:
    std::vector<sphere> _spheres;
    std::vector<triangle> _triangles;
    vec3 _light;
};