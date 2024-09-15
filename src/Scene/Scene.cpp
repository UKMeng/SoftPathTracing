//
// Created by UKMeng on 2024/8/18.
//

#include "Scene.h"

std::optional<HitInfo> Scene::Intersect(const Ray &ray, float tMin, float tMax) const
{
    return bvh->Intersect(ray, tMin, tMax);
}

void Scene::AddObject(const Object& object, Material* material, const Vec3f &translate, const Vec3f &rotate, const Vec3f &scale)
{
    // Model Matrix - from model space to world space
    Mat4f modelMatrix = Mat4f::Translate(translate) * Mat4f::Rotate(rotate) * Mat4f::Scale(scale);
    Mat4f invModelMatrix = modelMatrix.Inverse();
    m_ObjectList.emplace_back(new ObjectInstance(object, material, modelMatrix, invModelMatrix));
}
