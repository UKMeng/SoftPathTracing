//
// Created by UKMeng on 2024/9/4.
//

#include <algorithm>
#include <stack>

#include "SceneBVH.h"
#include "Profile.h"
#include "DebugMacro.h"

SceneBVH::SceneBVH(std::vector<ObjectInstance*>&& objects, int maxPrimsInNode, SceneBVH::SplitMethod splitMethod)
        : m_MaxPrimsInNode(maxPrimsInNode), m_SplitMethod(splitMethod)
{
    Profile profile("Build Scene BVH");
    SceneBVHTreeNode* root = m_Allocator.Allocate();

    auto tempInstances = std::move(objects);

    for (auto& instance: tempInstances)
    {
        if (instance->object.GetAABB().IsVaild())
        {
            instance->UpdateBounds();
            root->objects.push_back(instance);
        }
        else
        {
            m_InfiniteInstances.push_back(instance);
        }
    }

    root->UpdateBounds();
    root->depth = 1;

    SceneBVHState bvhState {};
    size_t totalPrimsCount = root->objects.size();

    switch (splitMethod)
    {
        case SplitMethod::NAIVE:
            Split(root, bvhState);
            break;
        case SplitMethod::SAH:
            SAHSplit(root, bvhState);
            break;
    }

    std::cout << "Total node count: " << bvhState.totalNodeCount << std::endl;
    std::cout << "Leaf node count: " << bvhState.leafNodeCount << std::endl;
    std::cout << "Total primitives count: " << totalPrimsCount << std::endl;
    std::cout << "Max leaf node primitives count: " << bvhState.maxLeafNodePrimsCount << std::endl;
    std::cout << "Mean leaf node primitives count: " << static_cast<float>(totalPrimsCount) / static_cast<float>(bvhState.leafNodeCount) << std::endl;
    std::cout << "Max leaf depth: " << bvhState.maxLeafDepth << std::endl;

    m_Nodes.reserve(bvhState.totalNodeCount);
    m_OrderedInstances.reserve(totalPrimsCount);
    Flatten(root);
}

void SceneBVH::Split(SceneBVHTreeNode *node, SceneBVHState& state)
{
    state.totalNodeCount++;
    if (node->objects.size() <= m_MaxPrimsInNode || node->depth > 32)
    {
        state.addLeafNode(node);
        return;
    }

    int dim = node->bounds.MaxExtentDimension();
    node->splitAxis = dim;
    float mid = node->bounds.Centroid()[dim];
    std::vector<ObjectInstance *> leftObjects, rightObjects;
    for (const auto &object : node->objects)
    {
        if (object->worldBounds.Centroid()[dim] < mid) leftObjects.push_back(object);
        else rightObjects.push_back(object);
    }
    if (leftObjects.empty() || rightObjects.empty())
    {
        state.addLeafNode(node);
        return;
    }

    auto* left = m_Allocator.Allocate();
    auto* right = m_Allocator.Allocate();
    node->children[0] = left;
    node->children[1] = right;
    left->objects = std::move(leftObjects);
    right->objects = std::move(rightObjects);
    left->UpdateBounds();
    right->UpdateBounds();
    left->depth = node->depth + 1;
    right->depth = node->depth + 1;

    node->objects.clear();
    node->objects.shrink_to_fit();

    Split(left, state);
    Split(right, state);
}

void SceneBVH::SAHSplit(SceneBVHTreeNode *node, SceneBVHState &state)
{
    state.totalNodeCount++;
    if (node->objects.size() <= m_MaxPrimsInNode || node->depth > 32)
    {
        state.addLeafNode(node);
        return;
    }

    Vec3f diag = node->bounds.Diagonal();

    float minCost = FLT_MAX;

    int bucketNum = 12; // min(objects.size(), 12);
    std::vector<std::vector<size_t>> indicesInBuckets(3 * bucketNum);
    size_t splitBucketIndex = 0;
    AABB minLeftBounds {};
    AABB minRightBounds {};
    size_t minLeftObjectsCount = 0;
    size_t minRightObjectsCount = 0;

    for (int dim = 0; dim < 3; dim++)
    {
        std::vector<AABB> bucketBounds(bucketNum);
        std::vector<size_t> objectsCountInBucket(bucketNum, 0);
        size_t objectIndex = 0;
        for (const auto& object: node->objects)
        {
            float axisCenter = object->worldBounds.Centroid()[dim];
            size_t bucketIndex = Clamp<size_t>(static_cast<size_t>((axisCenter - node->bounds.pMin[dim]) / diag[dim] * bucketNum), 0, bucketNum - 1);
            bucketBounds[bucketIndex] = bucketBounds[bucketIndex].Union(object->worldBounds);
            objectsCountInBucket[bucketIndex]++;
            indicesInBuckets[dim * bucketNum + bucketIndex].emplace_back(objectIndex);
            objectIndex++;
        }

        AABB leftBounds = bucketBounds[0];
        size_t leftObjectsCount = objectsCountInBucket[0];
        for (size_t i = 1; i < bucketNum; i++)
        {
            AABB rightBounds {};
            size_t rightObjectsCount = 0;
            for (size_t j = bucketNum - 1; j >= i; j--)
            {
                rightBounds = rightBounds.Union(bucketBounds[j]);
                rightObjectsCount += objectsCountInBucket[j];
            }
            if (rightObjectsCount == 0) break;
            if (leftObjectsCount != 0)
            {
                float cost = leftBounds.SurfaceArea() * leftObjectsCount + rightBounds.SurfaceArea() * rightObjectsCount;
                if (cost < minCost)
                {
                    minCost = cost;
                    node->splitAxis = dim;
                    splitBucketIndex = i;
                    minLeftBounds = leftBounds;
                    minRightBounds = rightBounds;
                    minLeftObjectsCount = leftObjectsCount;
                    minRightObjectsCount = rightObjectsCount;
                }
            }
            leftBounds = leftBounds.Union(bucketBounds[i]);
            leftObjectsCount += objectsCountInBucket[i];
        }
    }

    if (splitBucketIndex == 0)
    {
        state.addLeafNode(node);
        return;
    }

    auto* left = m_Allocator.Allocate();
    auto* right = m_Allocator.Allocate();
    node->children[0] = left;
    node->children[1] = right;

    left->objects.reserve(minLeftObjectsCount);
    right->objects.reserve(minRightObjectsCount);
    for (size_t i = 0; i < splitBucketIndex; i++)
    {
        for (size_t index: indicesInBuckets[node->splitAxis * bucketNum + i])
        {
            left->objects.push_back(node->objects[index]);
        }
    }
    for (size_t i = splitBucketIndex; i < bucketNum; i++)
    {
        for (size_t index: indicesInBuckets[node->splitAxis * bucketNum + i])
        {
            right->objects.push_back(node->objects[index]);
        }
    }

    left->bounds = minLeftBounds;
    right->bounds = minRightBounds;
    left->depth = node->depth + 1;
    right->depth = node->depth + 1;

    node->objects.clear();
    node->objects.shrink_to_fit();

    SAHSplit(left, state);
    SAHSplit(right, state);
}

size_t SceneBVH::Flatten(SceneBVHTreeNode *node)
{
    SceneBVHNode bvhNode {
            node->bounds,
            0,
            static_cast<uint16_t>(node->objects.size()),
            static_cast<uint8_t>(node->splitAxis)
    };

    size_t index = m_Nodes.size();
    m_Nodes.push_back(bvhNode);
    if (bvhNode.objectCount == 0)
    {
        Flatten(node->children[0]);
        m_Nodes[index].child1Index = Flatten(node->children[1]);
    }
    else
    {
        m_Nodes[index].objectIndex = m_OrderedInstances.size();
        for (const auto &object : node->objects)
        {
            m_OrderedInstances.push_back(object);
        }
    }
    return index;
}

std::optional<HitInfo> SceneBVH::Intersect(const Ray &ray, float tMin, float tMax) const
{
    std::optional<HitInfo> closestHitInfo {};
    ObjectInstance* closestInstance = nullptr;

    DEBUG_LINE(size_t boundsTestCount = 0)

    std::array<int, 32> stack;
    auto ptr = stack.begin();
    size_t currentIndex = 0;

    while (true)
    {
        auto& node = m_Nodes[currentIndex];

        DEBUG_LINE(boundsTestCount++)
        if (!node.bounds.HasIntersect(ray, tMin, tMax))
        {
            if (ptr == stack.begin()) break;
            currentIndex = *(--ptr);
            continue;
        }

        if (node.objectCount == 0)
        {
            int splitAxis = node.splitAxis;
            if (ray.direction[splitAxis] > 0)
            {
                currentIndex++;
                *(ptr++) = node.child1Index;
            }
            else
            {
                *(ptr++) = currentIndex + 1;
                currentIndex = node.child1Index;
            }
        }
        else
        {
            auto objectIter = m_OrderedInstances.begin() + node.objectIndex;
            for (size_t i = 0; i < node.objectCount; i++)
            {
                auto objectInstance = *objectIter++;
                Ray rayInModelSpace = ray.RayFromWorldToModel(objectInstance->invModelMatrix);
                auto hitInfo = objectInstance->object.Intersect(rayInModelSpace, tMin, tMax);
                DEBUG_LINE(ray.boundsTestCount += rayInModelSpace.boundsTestCount);
                DEBUG_LINE(ray.triangleTestCount += rayInModelSpace.triangleTestCount)
                if (hitInfo.has_value())
                {
                    tMax = hitInfo->t;
                    closestHitInfo = hitInfo;
                    closestInstance = objectInstance;
                }
            }
            if (ptr == stack.begin()) break;
            currentIndex = *(--ptr);
        }
    }

    for (const auto& infiniteInstance: m_InfiniteInstances)
    {
        Ray rayInModelSpace = ray.RayFromWorldToModel(infiniteInstance->invModelMatrix);
        auto hitInfo = infiniteInstance->object.Intersect(rayInModelSpace, tMin, tMax);
        DEBUG_LINE(ray.boundsTestCount += rayInModelSpace.boundsTestCount);
        DEBUG_LINE(ray.triangleTestCount += rayInModelSpace.triangleTestCount)
        if (hitInfo.has_value())
        {
            tMax = hitInfo->t;
            closestHitInfo = hitInfo;
            closestInstance = infiniteInstance;
        }
    }

    DEBUG_LINE(ray.boundsTestCount += boundsTestCount)

    if (closestInstance)
    {
        closestHitInfo->hitPos = closestInstance->modelMatrix * Vec4f(closestHitInfo->hitPos, 1.0f);
        closestHitInfo->normal = Normalize((closestInstance->invModelMatrix.Transpose() * Vec4f(closestHitInfo->normal, 0.0f)).xyz()); // use Normal Matrix
        closestHitInfo->material = closestInstance->material;
        closestHitInfo->object = closestInstance;
    }

    return closestHitInfo;
}
