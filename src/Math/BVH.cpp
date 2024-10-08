//
// Created by UKMeng on 2024/9/4.
//

#include <algorithm>

#include "BVH.h"
#include "Profile.h"
#include "DebugMacro.h"
#include "MyMath.h"

BVH::BVH(std::vector<Object *> &&objects, int maxPrimsInNode, SplitMethod splitMethod)
    : m_MaxPrimsInNode(maxPrimsInNode), m_SplitMethod(splitMethod)
{
    Profile profile("Build BVH");
    BVHTreeNode* root = m_Allocator.Allocate();
    root->objects = std::move(objects);
    root->UpdateBounds();
    root->depth = 1;

    BVHState bvhState {};
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
    m_OrderedPrimitives.reserve(totalPrimsCount);
    Flatten(root);
}

float BVH::CalArea(BVHTreeNode* node)
{
    float area = 0.0f;
    for (const auto &object : node->objects)
    {
        area += object->GetArea();
    }
    return area;
}

void BVH::Split(BVHTreeNode *node, BVHState& state)
{
    state.totalNodeCount++;
    if (node->objects.size() <= m_MaxPrimsInNode || node->depth > 32)
    {
        state.addLeafNode(node);
        node->area = CalArea(node);
        return;
    }

    int dim = node->bounds.MaxExtentDimension();
    node->splitAxis = dim;
    float mid = node->bounds.Centroid()[dim];
    std::vector<Object *> leftObjects, rightObjects;
    for (const auto &object : node->objects)
    {
        if (object->GetAABB().Centroid()[dim] < mid) leftObjects.push_back(object);
        else rightObjects.push_back(object);
    }
    if (leftObjects.empty() || rightObjects.empty())
    {
        state.addLeafNode(node);
        node->area = CalArea(node);
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

    node->area = left->area + right->area;
}

void BVH::SAHSplit(BVHTreeNode *node, BVHState &state)
{
    state.totalNodeCount++;
    if (node->objects.size() <= m_MaxPrimsInNode || node->depth > 32)
    {
        state.addLeafNode(node);
        node->area = CalArea(node);
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
            float axisCenter = object->GetAABB().Centroid()[dim];
            size_t bucketIndex = Clamp<size_t>(static_cast<size_t>((axisCenter - node->bounds.pMin[dim]) / diag[dim] * bucketNum), 0, bucketNum - 1);
            bucketBounds[bucketIndex] = bucketBounds[bucketIndex].Union(object->GetAABB());
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
        node->area = CalArea(node);
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

    node->area = left->area + right->area;
}

std::optional<HitInfo> BVH::Intersect(const Ray &ray, float tMin, float tMax) const
{
    std::optional<HitInfo> closestHitInfo {};

    DEBUG_LINE(size_t boundsTestCount = 0, triangleTestCount = 0)

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
            DEBUG_LINE(triangleTestCount += node.objectCount)
            auto objectIter = m_OrderedPrimitives.begin() + node.objectIndex;
            for (size_t i = 0; i < node.objectCount; i++)
            {
                auto object = *objectIter++;
                auto hitInfo = object->Intersect(ray, tMin, tMax);
                if (hitInfo.has_value())
                {
                    tMax = hitInfo->t;
                    closestHitInfo = hitInfo;
                }
            }
            if (ptr == stack.begin()) break;
            currentIndex = *(--ptr);
        }
    }

    DEBUG_LINE(ray.boundsTestCount += boundsTestCount)
    DEBUG_LINE(ray.triangleTestCount += triangleTestCount)

    return closestHitInfo;
}

size_t BVH::Flatten(BVHTreeNode *node)
{
    BVHNode bvhNode {
        node->bounds,
        node->area,
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
        m_Nodes[index].objectIndex = m_OrderedPrimitives.size();
        for (const auto &object : node->objects)
        {
            m_OrderedPrimitives.push_back(object);
        }
    }
    return index;
}

std::optional<HitInfo> BVH::Sample(RNG &rng)
{
    std::optional<HitInfo> hitInfo {};

    float p = rng.Uniform() * m_Nodes[0].area;

    size_t currentIndex = 0;
    while (true)
    {
        auto& node = m_Nodes[currentIndex];

        if (node.objectCount == 0)
        {
            size_t rightIndex = m_Nodes[currentIndex].child1Index;
            currentIndex++; // left node index;
            if (p >= m_Nodes[currentIndex].area) // if p is in the right node
            {
                p = p - m_Nodes[currentIndex].area;
                currentIndex = rightIndex;
            }
        }
        else
        {
            // leaf node
            auto objectIter = m_OrderedPrimitives.begin() + node.objectIndex;
            for (size_t i = 0; i < node.objectCount; i++)
            {
                auto object = *objectIter++;
                if (p < object->GetArea())
                {
                    hitInfo = object->Sample(rng);
                    break;
                }
                else
                {
                    p -= object->GetArea();
                }
            }
            break;
        }
    }
    return hitInfo;
}

//BVH::BVH(std::vector<Object *> &objects, int maxPrimsInNode, BVH::SplitMethod splitMethod)
//    : m_Primitives(std::move(objects)), m_MaxPrimsInNode(maxPrimsInNode), m_SplitMethod(splitMethod)
//{
//    Profile profile("Build BVH");
//    if (m_Primitives.empty()) return;
//
//    switch (splitMethod)
//    {
//        case SplitMethod::NAIVE:
//            root = Build(m_Primitives, 1);
//            break;
//        case SplitMethod::SAH:
//            root = SAHBuild(m_Primitives, 1);
//            break;
//    }
//
//    std::cout << "Max depth: " << m_MaxDepth << std::endl;
//}
//
//BVHTreeNode *BVH::Build(std::vector<Object *> objects, int depth)
//{
//    BVHTreeNode* node = new BVHTreeNode();
//    node->depth = depth;
//    m_MaxDepth = std::max(m_MaxDepth, depth);
//
//    AABB boundingBox {}; // bounding box for all objects
//    AABB centroidBoundingBox {};
//    for (auto &object : objects)
//    {
//        boundingBox = boundingBox.Union(object->GetAABB());
//        centroidBoundingBox = centroidBoundingBox.Union(object->GetAABB().Centroid());
//    }
//
//    if (objects.size() <= m_MaxPrimsInNode)
//    {
//        node->boundingBox = boundingBox;
//        node->left = nullptr;
//        node->right = nullptr;
//        node->objects = std::move(objects);
//        return node;
//    }
//    else
//    {
//        // sort objects by max extent dimension
//        int dim = centroidBoundingBox.MaxExtentDimension();
//        node->splitAxis = dim;
//        switch (dim)
//        {
//            case 0:
//                std::sort(objects.begin(), objects.end(), [](auto o1, auto o2)
//                {
//                    return o1->GetAABB().Centroid().x < o2->GetAABB().Centroid().x;
//                });
//                break;
//            case 1:
//                std::sort(objects.begin(), objects.end(), [](auto o1, auto o2)
//                {
//                    return o1->GetAABB().Centroid().y < o2->GetAABB().Centroid().y;
//                });
//                break;
//            case 2:
//                std::sort(objects.begin(), objects.end(), [](auto o1, auto o2)
//                {
//                    return o1->GetAABB().Centroid().z < o2->GetAABB().Centroid().z;
//                });
//                break;
//        }
//
//        auto begin = objects.begin();
//        auto mid = objects.begin() + objects.size() / 2;
//        auto end = objects.end();
//
//        auto leftObjects = std::vector<Object *>(begin, mid);
//        auto rightObjects = std::vector<Object *>(mid, end);
//
//        node->left = Build(leftObjects, depth + 1);
//        node->right = Build(rightObjects, depth + 1);
//        node->boundingBox = boundingBox;
//        return node;
//    }
//}
//
//BVHTreeNode *BVH::SAHBuild(std::vector<Object *> objects, int depth)
//{
//    BVHTreeNode* node = new BVHTreeNode();
//    node->depth = depth;
//    m_MaxDepth = std::max(m_MaxDepth, depth);
//
//    struct Bucket
//    {
//        int startIndex;
//        int endIndex;   // [start, end)
//        AABB boundingBox;
//    };
//
//    AABB boundingBox {}; // bounding box for all objects
//    AABB centroidBoundingBox {};
//    for (auto &object : objects)
//    {
//        boundingBox = boundingBox.Union(object->GetAABB());
//        centroidBoundingBox = centroidBoundingBox.Union(object->GetAABB().Centroid());
//    }
//
//    if (objects.size() <= m_MaxPrimsInNode)
//    {
//        node->boundingBox = boundingBox;
//        node->left = nullptr;
//        node->right = nullptr;
//        node->objects = std::move(objects);
//        return node;
//    }
//    else
//    {
//        int bucketNum = std::min<int>(objects.size(), 16);
//        int middle = 0;
//        float bestCost = FLT_MAX;
//
//        // init buckets
//        std::vector<std::vector<Bucket>> totalBuckets(3, std::vector<Bucket>(bucketNum));
//        for (int axis = 0; axis < 3; axis++)
//        {
//            for (int i = 0; i < bucketNum; ++i)
//            {
//                Vec3f bucketPMax = boundingBox.pMax;
//                bucketPMax[axis] = boundingBox.pMin[axis] + (bucketPMax[axis] - boundingBox.pMin[axis]) * static_cast<float>(i + 1) / static_cast<float>(bucketNum);
//                totalBuckets[axis][i].boundingBox = AABB(boundingBox.pMin, bucketPMax);
//            }
//        }
//
//        for (int axis = 0; axis < 3; axis++)
//        {
//            std::vector<Bucket>& buckets = totalBuckets[axis];
//
//            switch (axis) {
//                case 0:
//                    std::sort(objects.begin(), objects.end(), [](auto o1, auto o2)
//                    {
//                        return o1->GetAABB().Centroid().x < o2->GetAABB().Centroid().x;
//                    });
//                    break;
//                case 1:
//                    std::sort(objects.begin(), objects.end(), [](auto o1, auto o2)
//                    {
//                        return o1->GetAABB().Centroid().y < o2->GetAABB().Centroid().y;
//                    });
//                    break;
//                case 2:
//                    std::sort(objects.begin(), objects.end(), [](auto o1, auto o2)
//                    {
//                        return o1->GetAABB().Centroid().z < o2->GetAABB().Centroid().z;
//                    });
//                    break;
//            }
//
//            // compute bucket
//            int objectLeft = 0, objectRight = objects.size() - 1;
//
//            for (int i = 0; i < bucketNum; ++i)
//            {
//                buckets[i].startIndex = objectLeft;
//                while (objectLeft <= objectRight)
//                {
//                    int mid = objectLeft + (objectRight - objectLeft) / 2;
//
//                    if (AABB::Inside(objects[mid]->GetAABB().Centroid(), buckets[i].boundingBox))
//                    {
//                        objectLeft = mid + 1;
//                    }
//                    else
//                    {
//                        objectRight = mid - 1;
//                    }
//                }
//                buckets[i].endIndex = objectLeft;
//                objectRight = objects.size() - 1;
//            }
//
//            // calculate cost
//            int leftObjectNum = 0;
//            for (int i = 1; i < bucketNum; ++i)
//            {
//                leftObjectNum += buckets[i-1].endIndex - buckets[i-1].startIndex;
//
//                if (leftObjectNum == 0 || leftObjectNum == objects.size()) continue;
//
//                AABB leftBounds, rightBounds;
//                for (int j = 0; j < leftObjectNum; ++j)
//                {
//                    leftBounds = leftBounds.Union(objects[j]->GetAABB());
//                }
//                for (int j = leftObjectNum; j < objects.size(); ++j)
//                {
//                    rightBounds = rightBounds.Union(objects[j]->GetAABB());
//                }
//
//                double leftArea = leftBounds.SurfaceArea(), rightArea = rightBounds.SurfaceArea();
//                float invDiv = 1.0 / (leftArea + rightArea);
//
//                // float cost = invDiv * (i * leftObjectNum + (bucketNum - i) * (objects.size() - leftObjectNum));
//                float cost = invDiv * (leftObjectNum * leftArea + (objects.size() - leftObjectNum) * rightArea);
//                if (cost < bestCost)
//                {
//                    bestCost = cost;
//                    middle = leftObjectNum;
//                    node->splitAxis = axis;
//                }
//            }
//        }
//        auto begin = objects.begin();
//        auto mid = objects.begin() + middle;
//        auto end = objects.end();
//
//        auto leftObjects = std::vector<Object *>(begin, mid);
//        auto rightObjects = std::vector<Object *>(mid, end);
//
//        node->left = Build(leftObjects, depth + 1);
//        node->right = Build(rightObjects, depth + 1);
//        node->boundingBox = boundingBox;
//        return node;
//    }
//}

//std::optional<HitInfo> BVH::Intersect(const Ray &ray, float tMin, float tMax) const
//{
//    std::optional<HitInfo> closetHitInfo {};
//
//    DEBUG_LINE(m_BoundsTestCount = 0, m_TriangleTestCount = 0)
//
//    RecursiveIntersection(root, ray, tMin, tMax, closetHitInfo);
//
//    DEBUG_LINE(ray.boundsTestCount += m_BoundsTestCount)
//    DEBUG_LINE(ray.triangleTestCount += m_TriangleTestCount)
//
//    return closetHitInfo;
//}

//void BVH::RecursiveIntersection(BVHTreeNode *node, const Ray &ray, float tMin, float& tMax,
//                                std::optional<HitInfo> &closetHitInfo) const
//{
//    DEBUG_LINE(m_BoundsTestCount++)
//
//    if (!node->bounds.HasIntersect(ray, tMin, tMax)) return;
//
//    if (node->objects.empty())
//    {
//        int splitAxis = node->splitAxis;
//        if (ray.direction[splitAxis] > 0)
//        {
//            RecursiveIntersection(node->children[0], ray, tMin, tMax, closetHitInfo);
//            RecursiveIntersection(node->children[1], ray, tMin, tMax, closetHitInfo);
//        }
//        else
//        {
//            RecursiveIntersection(node->children[1], ray, tMin, tMax, closetHitInfo);
//            RecursiveIntersection(node->children[0], ray, tMin, tMax, closetHitInfo);
//        }
//    }
//    else
//    {
//        DEBUG_LINE(m_TriangleTestCount += node->objects.size())
//        for (auto &object: node->objects)
//        {
//            auto hitInfo = object->Intersect(ray, tMin, tMax);
//            if (hitInfo.has_value())
//            {
//                tMax = hitInfo->t;
//                closetHitInfo = hitInfo;
//            }
//        }
//    }
//}
