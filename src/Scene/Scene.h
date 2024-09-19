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
                   Material* material = nullptr,
                   const Vec3f& translate = {0.f, 0.f, 0.f},
                   const Vec3f& rotate = {0.f, 0.f, 0.f},
                   const Vec3f& scale = {1.f, 1.f, 1.f});

    virtual std::optional<HitInfo> Sample(RNG& rng) const;

    void BuildBVH()
    {
        bvh = std::make_unique<SceneBVH>(std::move(m_ObjectList));
    }

    virtual AABB GetAABB() const override
    {
        return bvh->GetAABB();
    }

    // return sum of emissive object area
    virtual float GetArea() const override
    {
        return m_EmissiveArea;
    }
private:
    std::vector<ObjectInstance*> m_ObjectList;
    std::vector<ObjectInstance*> m_EmissiveObjectList;
    std::unique_ptr<SceneBVH> bvh;
    float m_EmissiveArea = 0.0f;
};
