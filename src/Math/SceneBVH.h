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
    AABB worldBoundingBox = {};

    ObjectInstance(const Object& object, const Material& material, const Mat4f& modelMatrix, const Mat4f& invModelMatrix)
        : object(object), material(material), modelMatrix(modelMatrix), invModelMatrix(invModelMatrix) {}

    // Get AABB in world space
    AABB GetAABB()
    {
        if (worldBoundingBox.IsVaild()) return worldBoundingBox;
        AABB modelAABB = object.GetAABB();
        worldBoundingBox = modelAABB.Transform(modelMatrix);
        return worldBoundingBox;
    }
};

struct SceneBVHNode
{
    AABB boundingBox;
    SceneBVHNode* left;
    SceneBVHNode* right;
    std::vector<ObjectInstance*> objects;
    int depth;
    int splitAxis;

    SceneBVHNode()
    {
        boundingBox = AABB();
        left = nullptr;
        right = nullptr;
        objects = {};
    }
};

class SceneBVH
{
public:
    enum class SplitMethod
    {
        NAIVE,
        SAH
    };

    SceneBVH(std::vector<ObjectInstance*>& objects, int maxPrimsInNode = 1, SplitMethod splitMethod = SplitMethod::NAIVE);

    std::optional<HitInfo> Intersect(const Ray& ray, float tMin, float tMax) const;

public:
    SceneBVHNode* root;
private:
    SceneBVHNode* Build(std::vector<ObjectInstance*> objects, int depth);
//    SceneBVHNode* SAHBuild(std::vector<ObjectInstance*> objects, int depth);
//    void RecursiveIntersection(SceneBVHNode* node, const Ray& ray, float tMin, float& tMax, std::optional<HitInfo>& closetHitInfo) const;
private:
    const int m_MaxPrimsInNode;
    const SplitMethod m_SplitMethod;
    std::vector<ObjectInstance*> m_Primitives;
    int m_MaxDepth = 0;
};
