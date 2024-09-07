//
// Created by UKMeng on 2024/9/4.
//

#include <algorithm>

#include "BVH.h"
#include "Profile.h"

BVH::BVH(std::vector<Object *> &objects, int maxPrimsInNode, BVH::SplitMethod splitMethod)
    : m_Primitives(std::move(objects)), m_MaxPrimsInNode(maxPrimsInNode), m_SplitMethod(splitMethod)
{
    Profile profile("Build BVH");
    if (m_Primitives.empty()) return;
    root = Build(m_Primitives, 1);

    std::cout << "Max depth: " << m_MaxDepth << std::endl;
}

BVHNode *BVH::Build(std::vector<Object *> objects, int depth)
{
    BVHNode* node = new BVHNode();
    node->depth = depth;
    m_MaxDepth = std::max(m_MaxDepth, depth);

    AABB boundingBox {}; // bounding box for all objects
    for (auto &object : objects)
    {
        boundingBox = boundingBox.Union(object->GetAABB());
    }

    if (objects.size() == m_MaxPrimsInNode)
    {
        node->boundingBox = boundingBox;
        node->left = nullptr;
        node->right = nullptr;
        node->objects = std::move(objects);
        return node;
    }
    else
    {
        AABB centroidBoundingBox {};
        for (auto &object : objects)
        {
            centroidBoundingBox = centroidBoundingBox.Union(object->GetAABB().Centroid());
        }

        // sort objects by max extent dimension
        int dim = centroidBoundingBox.MaxExtentDimension();
        switch (dim)
        {
            case 0:
                std::sort(objects.begin(), objects.end(), [](auto o1, auto o2)
                {
                    return o1->GetAABB().Centroid().x < o2->GetAABB().Centroid().x;
                });
                break;
            case 1:
                std::sort(objects.begin(), objects.end(), [](auto o1, auto o2)
                {
                    return o1->GetAABB().Centroid().y < o2->GetAABB().Centroid().y;
                });
                break;
            case 2:
                std::sort(objects.begin(), objects.end(), [](auto o1, auto o2)
                {
                    return o1->GetAABB().Centroid().z < o2->GetAABB().Centroid().z;
                });
                break;
        }

        auto begin = objects.begin();
        auto mid = objects.begin() + objects.size() / 2;
        auto end = objects.end();

        auto leftObjects = std::vector<Object *>(begin, mid);
        auto rightObjects = std::vector<Object *>(mid, end);

        node->left = Build(leftObjects, depth + 1);
        node->right = Build(rightObjects, depth + 1);
        node->boundingBox = boundingBox;
        return node;
    }
}

std::optional<HitInfo> BVH::Intersect(const Ray &ray, float tMin, float tMax) const
{
    std::optional<HitInfo> closetHitInfo {};
    RecursiveIntersection(root, ray, tMin, tMax, closetHitInfo);
    return closetHitInfo;
}

void BVH::RecursiveIntersection(BVHNode *node, const Ray &ray, float tMin, float& tMax,
                                std::optional<HitInfo> &closetHitInfo) const
{
    if (!node->boundingBox.HasIntersect(ray, tMin, tMax)) return;

    if (node->objects.empty())
    {
        RecursiveIntersection(node->left, ray, tMin, tMax, closetHitInfo);
        RecursiveIntersection(node->right, ray, tMin, tMax, closetHitInfo);
    }
    else
    {
        for (auto &object: node->objects)
        {
            auto hitInfo = object->Intersect(ray, tMin, tMax);
            if (hitInfo.has_value())
            {
                tMax = hitInfo->t;
                closetHitInfo = hitInfo;
            }
        }
    }
}
