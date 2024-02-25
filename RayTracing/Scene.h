#pragma once

#include "Color.h"
#include "Sphere.h"
#include <vector>
#include <memory>

class Scene
{
public:
    Scene();

    [[nodiscard]] Color RayColor(const Ray& ray, unsigned int depth) const;

    void Add(const std::shared_ptr<Hittable>& object);
    void Clear();
    bool Hit(const Ray& ray, const Interval& rayInterval, HitRecord& record) const;

private:
    std::vector<std::shared_ptr<Hittable>> m_objects;
};
