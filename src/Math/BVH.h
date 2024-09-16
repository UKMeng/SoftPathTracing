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
    float area;

    void UpdateBounds()
    {
        bounds = {};
        for (auto& object: objects)
        {
            bounds = bounds.Union(object->GetAABB());
        }
    }
};

struct BVHNode
{
    AABB bounds;
    union
    {
        int objectIndex;
        int child1Index; // right child index in the array
    };
    uint16_t objectCount;
    uint8_t splitAxis;
    float area;
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

class BVHTreeNodeAllocator
{
public:
    BVHTreeNodeAllocator() : m_ptr(4096) {}
    ~BVHTreeNodeAllocator()
    {
        for (auto& node: m_NodeList)
        {
            delete[] node;
        }
        m_NodeList.clear();
    }

    BVHTreeNode* Allocate()
    {
        if (m_ptr == 4096)
        {
            m_NodeList.push_back(new BVHTreeNode[4096]);
            m_ptr = 0;
        }
        return &m_NodeList.back()[m_ptr++];
    }
private:
    size_t m_ptr;
    std::vector<BVHTreeNode*> m_NodeList;
};

class BVH
{
public:
    enum class SplitMethod
    {
        NAIVE,
        SAH
    };

    BVH(std::vector<Object*> &&objects, int maxPrimsInNode = 1, SplitMethod splitMethod = SplitMethod::SAH);

    std::optional<HitInfo> Intersect(const Ray& ray, float tMin, float tMax) const;

    std::optional<HitInfo> Sample(float& pdf, RNG& rng);

    AABB GetAABB() const
    {
        return m_Nodes[0].bounds;
    }

private:
    void Split(BVHTreeNode* node, BVHState& state);
    void SAHSplit(BVHTreeNode* node, BVHState& state);
    size_t Flatten(BVHTreeNode* node);
    float CalArea(BVHTreeNode* node);

//    BVHTreeNode* Build(std::vector<Object*> objects, int depth);
//    BVHTreeNode* SAHBuild(std::vector<Object*> objects, int depth);
//    void RecursiveIntersection(BVHTreeNode* node, const Ray& ray, float tMin, float& tMax, std::optional<HitInfo>& closetHitInfo) const;
private:
    std::vector<BVHNode> m_Nodes;
    const int m_MaxPrimsInNode;
    const SplitMethod m_SplitMethod;
    std::vector<Object*> m_OrderedPrimitives;
    BVHTreeNodeAllocator m_Allocator {};
};
