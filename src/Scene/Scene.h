//
// Created by UKMeng on 2024/8/18.
//

#pragma once
#include <vector>
#include "Object.h"
#include "Material.h"
#include "SceneBVH.h"

class Scene: public Object
{
public:
    virtual std::optional<HitInfo> Intersect(const Ray &ray, float tMin = 1e-5, float tMax = std::numeric_limits<float>::infinity()) const override;

    void AddObject(const Object& object,
                   const Material& material = {},
                   const Vec3f& translate = {0.f, 0.f, 0.f},
                   const Vec3f& rotate = {0.f, 0.f, 0.f},
                   const Vec3f& scale = {1.f, 1.f, 1.f});

    void BuildBVH()
    {
        bvh = std::make_unique<SceneBVH>(std::move(m_ObjectList));
    }

    virtual AABB GetAABB() const override
    {
        return bvh->GetAABB();
    }
private:
    std::vector<ObjectInstance*> m_ObjectList;
    std::unique_ptr<SceneBVH> bvh;
};
