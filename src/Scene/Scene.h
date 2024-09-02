//
// Created by UKMeng on 2024/8/18.
//

#pragma once
#include <vector>
#include "Object.h"
#include "Material.h"

struct ObjectInstance
{
    const Object& object;
    Material material;
    Mat4f modelMatrix;
    Mat4f invModelMatrix;
};


class Scene: public Object
{
public:
    virtual std::optional<HitInfo> Intersect(const Ray &ray, float tMin = 1e-5, float tMax = std::numeric_limits<float>::infinity()) const override;
    virtual AABB GetAABB() override;

    void AddObject(const Object& object,
                   const Material& material = {},
                   const Vec3f& translate = {0.f, 0.f, 0.f},
                   const Vec3f& rotate = {0.f, 0.f, 0.f},
                   const Vec3f& scale = {1.f, 1.f, 1.f});
private:
    std::vector<ObjectInstance> m_ObjectList;
};
