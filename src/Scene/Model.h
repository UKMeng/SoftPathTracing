//
// Created by UKMeng on 2024/8/18.
//

#pragma once
#include <vector>
#include <filesystem>
#include <rapidobj/rapidobj.hpp>

#include "Triangle.h"
#include "BVH.h"


class Model: public Object
{
public:
    std::vector<Triangle> triangles;

    Model(const std::filesystem::path &filename);

    virtual std::optional<HitInfo> Intersect(const Ray& ray, float tMin, float tMax) const override;

    virtual std::optional<HitInfo> Sample(RNG& rng) const override;

    virtual AABB GetAABB() const override;

    virtual float GetArea() const override
    {
        return area;
    }
private:
    std::unique_ptr<BVH> bvh;
    float area;
};
