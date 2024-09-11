//
// Created by UKMeng on 2024/8/18.
//

#pragma once
#include "MyMath.h"
#include "Ray.h"

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

    AABB Transform(const Mat4f& m) const;

    bool IsVaild() const { return pMax.x > pMin.x && pMax.y > pMin.y && pMax.z > pMin.z; }

    inline Vec3f GetCornerPoint(int index) const
    {
        return Vec3f(
                (index & 1) ? pMax.x : pMin.x,
                (index & 2) ? pMax.y : pMin.y,
                (index & 4) ? pMax.z : pMin.z
        );
    }

    bool HasIntersect(const Ray& ray, float tMin, float tMax) const;

    inline Vec3f Centroid() const { return 0.5 * pMin + 0.5 * pMax;}

    inline Vec3f Diagonal() const { return pMax - pMin;}

    inline int MaxExtentDimension() const
    {
        Vec3f diagonal = Diagonal();
        if (diagonal.x > diagonal.y && diagonal.x > diagonal.z) return 0;
        else if (diagonal.y > diagonal.z) return 1;
        else return 2;
    }

    inline float SurfaceArea() const
    {
        Vec3f d = Diagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
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

    static bool Inside(const Vec3f& p, const AABB& b)
    {
        return (p.x >= b.pMin.x && p.x <= b.pMax.x && p.y >= b.pMin.y &&
                p.y <= b.pMax.y && p.z >= b.pMin.z && p.z <= b.pMax.z);
    }
};
