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
    AABB boundingBox;

    Model(const std::filesystem::path &filename);

    virtual std::optional<HitInfo> Intersect(const Ray& ray, float tMin, float tMax) const override;

    virtual AABB GetAABB() override;
private:
    std::unique_ptr<BVH> bvh;
};
