//
// Created by UKMeng on 2024/8/18.
//

#include "Ray.h"

Ray Ray::RayFromWorldToModel(const Mat4f& worldToModel) const
{
    Vec3f o = worldToModel * Vec4f(origin, 1.f);
    Vec3f d = worldToModel * Vec4f(direction, 0.f);
    return Ray(o, d.Normalize());
}
