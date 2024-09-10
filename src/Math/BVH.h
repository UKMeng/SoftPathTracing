//
// Created by UKMeng on 2024/9/4.
//

#pragma once

#include <vector>

#include "AABB.h"
#include "Object.h"

struct BVHNode
{
    AABB boundingBox;
    BVHNode* left;
    BVHNode* right;
    std::vector<Object*> objects;
    int depth;

    BVHNode()
    {
        boundingBox = AABB();
        left = nullptr;
        right = nullptr;
        objects = {};
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

    BVH(std::vector<Object*>& objects, int maxPrimsInNode = 1, SplitMethod splitMethod = SplitMethod::NAIVE);

    std::optional<HitInfo> Intersect(const Ray& ray, float tMin, float tMax) const;

public:
    BVHNode* root;
private:
    BVHNode* Build(std::vector<Object*> objects, int depth);
    BVHNode* SAHBuild(std::vector<Object*> objects, int depth);
    void RecursiveIntersection(BVHNode* node, const Ray& ray, float tMin, float& tMax, std::optional<HitInfo>& closetHitInfo) const;
private:
    const int m_MaxPrimsInNode;
    const SplitMethod m_SplitMethod;
    std::vector<Object*> m_Primitives;
    int m_MaxDepth = 0;
};
