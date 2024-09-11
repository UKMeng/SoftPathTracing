//
// Created by UKMeng on 2024/9/4.
//

#include <algorithm>

#include "BVH.h"
#include "Profile.h"
#include "DebugMacro.h"

BVH::BVH(std::vector<Object *> &objects, int maxPrimsInNode, BVH::SplitMethod splitMethod)
    : m_Primitives(std::move(objects)), m_MaxPrimsInNode(maxPrimsInNode), m_SplitMethod(splitMethod)
{
    Profile profile("Build BVH");
    if (m_Primitives.empty()) return;

    switch (splitMethod)
    {
        case SplitMethod::NAIVE:
            root = Build(m_Primitives, 1);
            break;
        case SplitMethod::SAH:
            root = SAHBuild(m_Primitives, 1);
            break;
    }

    std::cout << "Max depth: " << m_MaxDepth << std::endl;
}

BVHNode *BVH::Build(std::vector<Object *> objects, int depth)
{
    BVHNode* node = new BVHNode();
    node->depth = depth;
    m_MaxDepth = std::max(m_MaxDepth, depth);

    AABB boundingBox {}; // bounding box for all objects
    AABB centroidBoundingBox {};
    for (auto &object : objects)
    {
        boundingBox = boundingBox.Union(object->GetAABB());
        centroidBoundingBox = centroidBoundingBox.Union(object->GetAABB().Centroid());
    }

    if (objects.size() <= m_MaxPrimsInNode)
    {
        node->boundingBox = boundingBox;
        node->left = nullptr;
        node->right = nullptr;
        node->objects = std::move(objects);
        return node;
    }
    else
    {
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

BVHNode *BVH::SAHBuild(std::vector<Object *> objects, int depth)
{
    BVHNode* node = new BVHNode();
    node->depth = depth;
    m_MaxDepth = std::max(m_MaxDepth, depth);

    struct Bucket
    {
        int startIndex;
        int endIndex;   // [start, end)
        AABB boundingBox;
    };

    AABB boundingBox {}; // bounding box for all objects
    AABB centroidBoundingBox {};
    for (auto &object : objects)
    {
        boundingBox = boundingBox.Union(object->GetAABB());
        centroidBoundingBox = centroidBoundingBox.Union(object->GetAABB().Centroid());
    }

    if (objects.size() <= m_MaxPrimsInNode)
    {
        node->boundingBox = boundingBox;
        node->left = nullptr;
        node->right = nullptr;
        node->objects = std::move(objects);
        return node;
    }
    else
    {
        int bucketNum = std::min<int>(objects.size(), 16);
        int middle = 0;
        float bestCost = FLT_MAX;

        // init buckets
        std::vector<std::vector<Bucket>> totalBuckets(3, std::vector<Bucket>(bucketNum));
        for (int axis = 0; axis < 3; axis++)
        {
            for (int i = 0; i < bucketNum; ++i)
            {
                Vec3f bucketPMax = boundingBox.pMax;
                bucketPMax[axis] = boundingBox.pMin[axis] + (bucketPMax[axis] - boundingBox.pMin[axis]) * static_cast<float>(i + 1) / static_cast<float>(bucketNum);
                totalBuckets[axis][i].boundingBox = AABB(boundingBox.pMin, bucketPMax);
            }
        }

        for (int axis = 0; axis < 3; axis++)
        {
            std::vector<Bucket>& buckets = totalBuckets[axis];

            switch (axis) {
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

            // compute bucket
            int objectLeft = 0, objectRight = objects.size() - 1;

            for (int i = 0; i < bucketNum; ++i)
            {
                buckets[i].startIndex = objectLeft;
                while (objectLeft <= objectRight)
                {
                    int mid = objectLeft + (objectRight - objectLeft) / 2;

                    if (AABB::Inside(objects[mid]->GetAABB().Centroid(), buckets[i].boundingBox))
                    {
                        objectLeft = mid + 1;
                    }
                    else
                    {
                        objectRight = mid - 1;
                    }
                }
                buckets[i].endIndex = objectLeft;
                objectRight = objects.size() - 1;
            }

            // calculate cost
            int leftObjectNum = 0;
            for (int i = 1; i < bucketNum; ++i)
            {
                leftObjectNum += buckets[i-1].endIndex - buckets[i-1].startIndex;

                if (leftObjectNum == 0 || leftObjectNum == objects.size()) continue;

                AABB leftBounds, rightBounds;
                for (int j = 0; j < leftObjectNum; ++j)
                {
                    leftBounds = leftBounds.Union(objects[j]->GetAABB());
                }
                for (int j = leftObjectNum; j < objects.size(); ++j)
                {
                    rightBounds = rightBounds.Union(objects[j]->GetAABB());
                }

                double leftArea = leftBounds.SurfaceArea(), rightArea = rightBounds.SurfaceArea();
                float invDiv = 1.0 / (leftArea + rightArea);

                // float cost = invDiv * (i * leftObjectNum + (bucketNum - i) * (objects.size() - leftObjectNum));
                float cost = invDiv * (leftObjectNum * leftArea + (objects.size() - leftObjectNum) * rightArea);
                if (cost < bestCost)
                {
                    bestCost = cost;
                    middle = leftObjectNum;
                }
            }
        }
        auto begin = objects.begin();
        auto mid = objects.begin() + middle;
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

    DEBUG_LINE(m_BoundsTestCount = 0, m_TriangleTestCount = 0)

    RecursiveIntersection(root, ray, tMin, tMax, closetHitInfo);

    if (closetHitInfo.has_value())
    {
        DEBUG_LINE(closetHitInfo->boundsTestCount = m_BoundsTestCount)
        DEBUG_LINE(closetHitInfo->triangleTestCount = m_TriangleTestCount)
    }

    return closetHitInfo;
}

void BVH::RecursiveIntersection(BVHNode *node, const Ray &ray, float tMin, float& tMax,
                                std::optional<HitInfo> &closetHitInfo) const
{
    DEBUG_LINE(m_BoundsTestCount++)

    if (!node->boundingBox.HasIntersect(ray, tMin, tMax)) return;

    if (node->objects.empty())
    {
        RecursiveIntersection(node->left, ray, tMin, tMax, closetHitInfo);
        RecursiveIntersection(node->right, ray, tMin, tMax, closetHitInfo);
    }
    else
    {
        DEBUG_LINE(m_TriangleTestCount += node->objects.size())
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