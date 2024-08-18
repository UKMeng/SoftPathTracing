//
// Created by UKMeng on 2024/8/18.
//

#pragma once
#include "MyMath.h"

class AABB
{
public:
    Vec3f pMin, pMax;

    AABB()
    {
        float minNum = std::numeric_limits<float>::lowest();
        float maxNum = std::numeric_limits<float>::max();
        pMin = Vec3f(maxNum, maxNum, maxNum);
        pMax = Vec3f(minNum, minNum, minNum);
    }
    AABB(const Vec3f p) : pMin(p), pMax(p) {}
    AABB(const Vec3f p1, const Vec3f p2)
    {
        pMin = Vec3f(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
        pMax = Vec3f(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
    }

    inline AABB Union(const AABB& b)
    {
        AABB ret;
        ret.pMin = Vec3f::Min(pMin, b.pMin);
        ret.pMax = Vec3f::Max(pMax, b.pMax);
        return ret;
    }

    inline AABB Union(const Vec3f& p)
    {
        AABB ret;
        ret.pMin = Vec3f::Min(pMin, p);
        ret.pMax = Vec3f::Max(pMax, p);
        return ret;
    }

};
