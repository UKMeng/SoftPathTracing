//
// Created by UKMeng on 2024/9/4.
//

#pragma once

#include <vector>

#include "AABB.h"
#include "Object.h"

struct ObjectInstance
{
    const Object& object;
    Material material;
    Mat4f modelMatrix;
    Mat4f invModelMatrix;
    AABB worldBounds = {};

    ObjectInstance(const Object& object, const Material& material, const Mat4f& modelMatrix, const Mat4f& invModelMatrix)
        : object(object), material(material), modelMatrix(modelMatrix), invModelMatrix(invModelMatrix) {}

    void UpdateBounds()
    {
        // Get AABB in world space
        AABB modelAABB = object.GetAABB();
        worldBounds = modelAABB.Transform(modelMatrix);
    }
};

struct SceneBVHTreeNode
{
    AABB bounds;
    SceneBVHTreeNode* children[2];
    std::vector<ObjectInstance*> objects;
    int depth;
    int splitAxis;

    void UpdateBounds()
    {
        bounds = {};
        for (auto& object: objects)
        {
            bounds = bounds.Union(object->worldBounds);
        }
    }
};

struct alignas(32) SceneBVHNode
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

struct SceneBVHState
{
    size_t totalNodeCount {};
    size_t leafNodeCount {};
    size_t maxLeafNodePrimsCount {};
    size_t maxLeafDepth {};

    void addLeafNode(SceneBVHTreeNode* node)
    {
        leafNodeCount++;
        maxLeafNodePrimsCount = std::max(maxLeafNodePrimsCount, node->objects.size());
        maxLeafDepth = std::max<size_t>(maxLeafDepth, node->depth);
    }
};

class SceneBVHTreeNodeAllocator
{
public:
    SceneBVHTreeNodeAllocator() : m_ptr(4096) {}
    ~SceneBVHTreeNodeAllocator()
    {
        for (auto& node: m_NodeList)
        {
            delete[] node;
        }
        m_NodeList.clear();
    }

    SceneBVHTreeNode* Allocate()
    {
        if (m_ptr == 4096)
        {
            m_NodeList.push_back(new SceneBVHTreeNode[4096]);
            m_ptr = 0;
        }
        return &m_NodeList.back()[m_ptr++];
    }
private:
    size_t m_ptr;
    std::vector<SceneBVHTreeNode*> m_NodeList;
};

class SceneBVH
{
public:
    enum class SplitMethod
    {
        NAIVE,
        SAH
    };

    SceneBVH(std::vector<ObjectInstance*>&& objects, int maxPrimsInNode = 1, SplitMethod splitMethod = SplitMethod::SAH);

    std::optional<HitInfo> Intersect(const Ray& ray, float tMin, float tMax) const;

    AABB GetAABB() const
    {
        return m_Nodes[0].bounds;
    }

private:
    void Split(SceneBVHTreeNode* node, SceneBVHState& state);
    void SAHSplit(SceneBVHTreeNode* node, SceneBVHState& state);
    size_t Flatten(SceneBVHTreeNode* node);


private:
    std::vector<SceneBVHNode> m_Nodes;
    const int m_MaxPrimsInNode;
    const SplitMethod m_SplitMethod;
    std::vector<ObjectInstance*> m_OrderedInstances;
    std::vector<ObjectInstance*> m_InfiniteInstances;
    SceneBVHTreeNodeAllocator m_Allocator {};
};
