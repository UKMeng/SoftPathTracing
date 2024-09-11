//
// Created by UKMeng on 2024/9/4.
//

#pragma once

#include <vector>

#include "AABB.h"
#include "Object.h"

struct BVHTreeNode
{
    AABB bounds;
    BVHTreeNode* children[2];
    std::vector<Object*> objects;
    int depth;
    int splitAxis;

    void UpdateBounds()
    {
        bounds = {};
        for (auto& object: objects)
        {
            bounds = bounds.Union(object->GetAABB());
        }
    }
};

struct alignas(32) BVHNode
{
    AABB bounds;
    union
    {
        int objectIndex;
        int child1Index; // right child index in the array
    };
    uint16_t objectCount;
    uint8_t splitAxis;
};

class BVH
{
public:
    enum class SplitMethod
    {
        NAIVE,
        SAH
    };

    BVH(std::vector<Object*> &&objects, int maxPrimsInNode = 1, SplitMethod splitMethod = SplitMethod::NAIVE);

//    BVH(std::vector<Object*>& objects, int maxPrimsInNode = 1, SplitMethod splitMethod = SplitMethod::NAIVE);

    std::optional<HitInfo> Intersect(const Ray& ray, float tMin, float tMax) const;

    AABB GetAABB() const
    {
        return m_Nodes[0].bounds;
    }

private:
    void Split(BVHTreeNode* node);
    size_t Flatten(BVHTreeNode* node);

//    BVHTreeNode* Build(std::vector<Object*> objects, int depth);
//    BVHTreeNode* SAHBuild(std::vector<Object*> objects, int depth);
//    void RecursiveIntersection(BVHTreeNode* node, const Ray& ray, float tMin, float& tMax, std::optional<HitInfo>& closetHitInfo) const;
private:
    std::vector<BVHNode> m_Nodes;
    const int m_MaxPrimsInNode;
    const SplitMethod m_SplitMethod;
    std::vector<Object*> m_OrderedPrimitives;
    int m_MaxDepth = 0;
};
