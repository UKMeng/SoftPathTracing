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
    auto* objectInstance = new ObjectInstance(object, material, modelMatrix, invModelMatrix);
    if (material->isEmissive)
    {
        m_EmissiveObjectList.emplace_back(objectInstance);
    }
    m_ObjectList.emplace_back(objectInstance);
}

std::optional<HitInfo> Scene::Sample(float &pdf, RNG& rng) const
{
    std::optional<HitInfo> hitInfo {};

    if (m_EmissiveObjectList.empty()) return hitInfo;

    // TODO(BUG FIX): area should be related to scaling matrix
    float emitAreaSum = 0.0f;
    for (const auto& objectInstance: m_EmissiveObjectList)
    {
        emitAreaSum += objectInstance->object.GetArea();
    }

    float p = rng.Uniform() * emitAreaSum;
    emitAreaSum = 0;

    for (const auto& objectInstance: m_EmissiveObjectList)
    {
        emitAreaSum += objectInstance->object.GetArea();
        if (p <= emitAreaSum)
        {
            hitInfo = objectInstance->object.Sample(pdf, rng);
            hitInfo->hitPos = objectInstance->modelMatrix * Vec4f(hitInfo->hitPos, 1.0f);
            hitInfo->normal = Normalize((objectInstance->invModelMatrix.Transpose() * Vec4f(hitInfo->normal, 0.0f)).xyz()); // use Normal Matrix
            hitInfo->material = objectInstance->material;
            break;
        }
    }

    return hitInfo;
}
