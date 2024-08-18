//
// Created by UKMeng on 2024/8/18.
//

#include "Scene.h"

std::optional<HitInfo> Scene::Intersect(const Ray &ray, float tMin, float tMax) const
{
    std::optional<HitInfo> closestHitInfo = {};
    const ObjectInstance* closestInstance = nullptr;

    for (const auto& objectInstance: m_ObjectList)
    {
        Ray rayInModelSpace = ray.RayFromWorldToModel(objectInstance.invModelMatrix);
        auto hitInfo = objectInstance.object->Intersect(rayInModelSpace, tMin, tMax);
        if (hitInfo.has_value())
        {
            closestHitInfo = hitInfo;
            tMax = hitInfo->t;
            closestInstance = &objectInstance;
        }
    }

    if (closestInstance)
    {
        closestHitInfo->hitPos = closestInstance->modelMatrix * Vec4f(closestHitInfo->hitPos, 1.0f);
        closestHitInfo->normal = Normalize((closestInstance->invModelMatrix.Transpose() * Vec4f(closestHitInfo->normal, 0.0f)).xyz()); // use Normal Matrix
    }
    return closestHitInfo;
}

void Scene::AddObject(const Object *object, const Vec3f &translate, const Vec3f &rotate, const Vec3f &scale)
{
    // Model Matrix - from model space to world space
    Mat4f modelMatrix = Mat4f::Translate(translate) * Mat4f::Rotate(rotate) * Mat4f::Scale(scale);
    Mat4f invModelMatrix = modelMatrix.Inverse();
    m_ObjectList.emplace_back(object, modelMatrix, invModelMatrix);
}

AABB Scene::GetAABB()
{
    return AABB();
}
