//
// Created by UKMeng on 2024/8/18.
//

#pragma once

#include "MyMath.h"

class Ray
{
public:
    Ray(Vec3f ori, Vec3f dir): m_Ori(ori), m_Dir(dir) {};

    const Vec3f& GetOrigin() const {return m_Ori;}
    const Vec3f& GetDirection() const {return m_Dir;}
private:
    Vec3f m_Ori, m_Dir;
};
