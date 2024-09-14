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

struct BVHState
{
    size_t totalNodeCount {};
    size_t leafNodeCount {};
    size_t maxLeafNodePrimsCount {};
    size_t maxLeafDepth {};

    void addLeafNode(BVHTreeNode* node)
    {
        leafNodeCount++;
        maxLeafNodePrimsCount = std::max(maxLeafNodePrimsCount, node->objects.size());
        maxLeafDepth = std::max<size_t>(maxLeafDepth, node->depth);
    }

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
    void Split(BVHTreeNode* node, BVHState& state);
    void SAHSplit(BVHTreeNode* node, BVHState& state);
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
